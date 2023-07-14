#include "../../headers/playlist/playlistmodel.h"

#include <QErrorMessage>
#include <QFile>

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractItemModel(parent)
{}

//Adding new media to playlist
void PlaylistModel::add(const QUrl &url)
{
    PlaylistMediaElement newElement(url);

    if(!m_mediaElements.contains(newElement) && newElement.isPathValid() && !newElement.isEmpty())
    {
        m_mediaElements.push_back(newElement);
        updateAllData();
    }
}

//Removing one media from playlist using url
void PlaylistModel::remove(const QUrl &url)
{
    for(int i = 0; i < m_mediaElements.size(); ++i)
    {
        const PlaylistMediaElement &element = m_mediaElements[i];

        if(element.mediaPath()== url)
        {
            m_mediaElements.remove(i);
            updateAllData();
            return;
        }
    }
}

void PlaylistModel::remove(const QList<QUrl> &urls)
{
    for(const QUrl& url : urls)
        qDebug() << url;

    for(int i = 0; i < m_mediaElements.size(); ++i)
    {
        const PlaylistMediaElement &element = m_mediaElements[i];

        if(urls.contains(element.mediaPath()))
        {
            m_mediaElements.remove(i--);
        }
    }
    updateAllData();
}

//Getting copy of playlist element by url
PlaylistMediaElement PlaylistModel::get(const QUrl &source)
{
    for(const PlaylistMediaElement &element : m_mediaElements)
    {
        if(element.mediaPath() == source)
            return element;
    }
    return PlaylistMediaElement();
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

//Simply return all song requested data by QMediaMetaData key.
QStringList PlaylistModel::getAllInfo(const QMediaMetaData::Key &key) const
{
    QStringList info;
    for(const QMediaMetaData &data : m_mediaElements)
    {
        info << data.value(key).toString();
    }
    return info;
}

bool PlaylistModel::loadFromFile(const QString &pathname)
{
    QFile file(pathname);
    if(!file.open(QIODevice::ReadOnly)) {
        return false;
    }


    QTextStream stream(&file);
    m_mediaElements.clear();
    QString url;
    while(stream.readLineInto(&url))
        m_mediaElements.push_back(PlaylistMediaElement(QUrl(url)));

    file.close();
    updateAllData();
    return true;
}

bool PlaylistModel::saveToFile(const QString &pathname)
{
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

