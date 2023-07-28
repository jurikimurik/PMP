#include "../../headers/playlist/playlistmodel.h"

#include <QErrorMessage>
#include <QFile>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractItemModel(parent)
{}

void PlaylistModel::add(const QUrl &url)
{
    Playlist::add(url);
    updateAllData();
}

void PlaylistModel::remove(const QList<QUrl> &urls)
{
    Playlist::remove(urls);
    updateAllData();
}

void PlaylistModel::remove(const QUrl &url)
{
    Playlist::remove(url);
    updateAllData();
}


void PlaylistModel::insert(const QUrl &url, const QModelIndex &after)
{
    if(!url.isValid())
        return;

    int newRow = after.row()+1;
    beginInsertRows(after, newRow, newRow);
    m_mediaElements.insert(newRow, PlaylistMediaElement(url));
    endInsertRows();

    updateAllData();
}

void PlaylistModel::insert(const QList<QUrl> &urls, const QModelIndex &after)
{
    QModelIndex changableIndex = after;
    for(const QUrl &url: urls) {
        if(changableIndex.isValid()) {
            insert(url, changableIndex);
            changableIndex = changableIndex.siblingAtRow(changableIndex.row()+1);
        } else {
            qDebug() << "QModelIndex in insert QList<QUrl> is wrong!";
            return;
        }
    }
}


QModelIndex PlaylistModel::indexPositionOf(const PlaylistMediaElement &element) const
{
    return index(positionOf(element), 0, QModelIndex());
}

//Getting playlist media source url using index in table!
//  - Searching using title only. (May delete something else if have same names)
//TODO: (getSourceURL) Better searching using more data.
QUrl PlaylistModel::getSourceURL(const QModelIndex &index) const
{
    QString mediaName = data(index.parent(), Qt::DisplayRole).toString();
    for(const PlaylistMediaElement& element : m_mediaElements)
    {
        if(mediaName == element.value(QMediaMetaData::Title).toString())
            return element.mediaPath();
    }
    return QUrl();
}

bool PlaylistModel::loadFromFile(const QString &pathname)
{
    QFile file(pathname);
    if(!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    //Checking for M3U
    QTextStream stream(&file);
    QString firstLine = stream.readLine();
    if(firstLine.toUpper().contains("#EXTM3U"))
    {
        stream.reset();
        file.close();
        return loadFromM3UFile(pathname);
    }

    stream.seek(0);
    m_mediaElements.clear();
    QString url;
    while(stream.readLineInto(&url)) {
        m_mediaElements.push_back(PlaylistMediaElement(QUrl(url)));
    }

    file.close();
    updateAllData();
    return true;
}

void PlaylistModel::setPlaylistName(const QString &newPlaylistName)
{
    if(newPlaylistName == m_playlistName)
        return;
    m_playlistName = newPlaylistName;
    emit playlistNameChanged(m_playlistName);
}

bool PlaylistModel::saveToFile(const QString &pathname)
{
    //For M3U file format
    if(pathname.split(".").last().toLower() == "m3u")
        return saveToM3UFile(pathname);

    QFile file(pathname);
    if(!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QTextStream stream(&file);
    for(const PlaylistMediaElement &element : m_mediaElements)
        stream << element.mediaPath().toString() << "\n";

    file.close();
    return true;
}

//IMPORTANT: !!!!!! AFTER EVERY MANUPILATION ON m_mediaElements THIS NEEDS TO BE CALLED !!!!!!
void PlaylistModel::updateAllData()
{
    beginResetModel();
    endResetModel();

    emit mediaChanged(Playlist(m_playlistName, m_mediaElements));
}

bool PlaylistModel::saveToM3UFile(const QString &pathname)
{
    QFile file(pathname);
    if(!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QTextStream stream(&file);
    stream << "#EXTM3U" << "\n";

    if(!m_playlistName.isEmpty())
        stream << "#PLAYLIST:" << m_playlistName << "\n";

    for(const PlaylistMediaElement &element : m_mediaElements)
    {
        int lengthInSeconds = element.value(QMediaMetaData::Duration).toInt() / 1000;
        QString artist = element.value(QMediaMetaData::AlbumArtist).toString();
        QString songName = element.value(QMediaMetaData::Title).toString();
        stream << "#EXTINF:" << lengthInSeconds << ",";
        if(!artist.isEmpty())
            stream << artist << " - ";
        stream << songName << "\n";
        stream << element.mediaPath().toString() << "\n";
    }

    file.close();
    return true;
}

bool PlaylistModel::loadFromM3UFile(const QString &pathname)
{
    QFile file(pathname);
    if(!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QTextStream stream(&file);
    QString line;
    QStringList m3uLines;
    //Every m3u line before media element will be connected with it.
    //  - After media element will be readed, all m3u lines will add information to media.
    while(stream.readLineInto(&line)) {
        if(line.contains("#")) {
            m3uLines.push_back(line);
            continue;
        }

        // Firstly, try to load URL
        QUrl url(line);
        if(!url.isValid()) {
            // If invalid, try to read as path to file
            url = QUrl::fromLocalFile(line);
            if(!url.isValid())
                // If even after that url is still invalid, skip
                continue;
        }

        PlaylistMediaElement element(url);
        if(!m3uLines.isEmpty())
            loadM3UInfoInto(m3uLines.join("\n"), element);
        m_mediaElements.push_back(element);
        m3uLines.clear();
    }
    updateAllData();
    return true;
}

void PlaylistModel::loadM3UInfoInto(const QString &line, PlaylistMediaElement &intoElement)
{
    if(line.isEmpty())
        return;

    QStringList list = line.split("\n");
    for(const QString &m3uStr : list)
    {
        //Default header
        if(m3uStr.toUpper().contains("#EXTM3U"))
            continue;

        if(m3uStr.toUpper().contains("#EXTINF:"))
        {
            QRegularExpression regex(R"(^\s*#EXTINF:(-{0,1}\d*),{0,1}(.*) - {0,1}(.*)|^\s*#EXTINF:(-{0,1}\d*),{0,1}(.*))");
            QRegularExpressionMatch match = regex.match(m3uStr);
            if(match.hasMatch())
            {
                int lengthInSeconds;
                QString artist;
                QString songName;


                QStringList captureTexts = match.capturedTexts();
                lengthInSeconds = captureTexts.value(1).toInt();

                if(captureTexts.size() == 4) {
                    //With artist
                    artist = captureTexts.value(2);
                    songName = captureTexts.value(3);
                } else {
                    //Without artist
                    lengthInSeconds = captureTexts.value(4).toInt();
                    songName = captureTexts.value(5);
                }



                //If there is no duration, artist or song name - insert it.
                if(intoElement.value(QMediaMetaData::Duration).toInt() && !lengthInSeconds)
                    intoElement.insert(QMediaMetaData::Duration, lengthInSeconds*1000);
                if(intoElement.value(QMediaMetaData::Title).toString().isEmpty() && !songName.isEmpty())
                    intoElement.insert(QMediaMetaData::Title, songName);
                if(intoElement.value(QMediaMetaData::AlbumArtist).toString().isEmpty() && !artist.isEmpty())
                    intoElement.insert(QMediaMetaData::AlbumArtist, artist);
            }
        }

        if(m3uStr.toUpper().contains("#PLAYLIST:"))
        {
            QRegularExpression regex(R"(^\s*#PLAYLIST:\s*(.*))");
            QRegularExpressionMatch match = regex.match(m3uStr);
            if(match.hasMatch())
            {
                //PLAYLIST property can be only once in a file.
                //  - If there will be more: last name will be accepted as playlist name.
                setPlaylistName(match.captured(1));
            }
        }
    }
}
//---------------------- Reimplementing all necessary methods for QAbstractItemModel below... ----------------------

QModelIndex PlaylistModel::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return createIndex(row, column);
}

QModelIndex PlaylistModel::parent(const QModelIndex &child) const
{
    return createIndex(child.row(),0);
}

int PlaylistModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_mediaElements.count();
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return QMetaEnum::fromType<QMediaMetaData::Key>().keyCount();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return m_mediaElements.value(index.row()).value((QMediaMetaData::Key) index.column());
    }

    return QVariant();
}

QVariant PlaylistModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();


    if(orientation == Qt::Orientation::Horizontal)
    {
        QMetaEnum metaEnum = QMetaEnum::fromType<QMediaMetaData::Key>();
        return metaEnum.key(section);
    } else {
        return section+1;
    }
}

