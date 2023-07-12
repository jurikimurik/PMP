#include "../../headers/playlist/playlistmodel.h"

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractItemModel(parent)
{}

void PlaylistModel::add(const QUrl &url)
{
    QMediaPlayer* tempPlayer = new QMediaPlayer(this);
    tempPlayer->setSource(url);

    QMediaMetaData metaData = tempPlayer->metaData();
    if(!m_mediaElements.contains(PlaylistMediaElement(metaData, tempPlayer->source())) &&
        (tempPlayer->error() == QMediaPlayer::NoError || tempPlayer->error() == QMediaPlayer::FormatError))
    {
        m_mediaElements.push_back(PlaylistMediaElement(metaData, tempPlayer->source()));
        updateAllData();
    }
}

void PlaylistModel::remove(const QString &title)
{
    for(int i = 0; i < m_mediaElements.size(); ++i)
    {
        QMediaMetaData &data = m_mediaElements[i];

        if(data.value(QMediaMetaData::Title).toString() == title)
        {
            m_mediaElements.remove(i);
            updateAllData();
            return;
        }
    }
}

void PlaylistModel::remove(const QUrl &url)
{
    for(int i = 0; i < m_mediaElements.size(); ++i)
    {
        QMediaMetaData &data = m_mediaElements[i];

        if(data.value(QMediaMetaData::Url).toUrl() == url)
        {
            m_mediaElements.remove(i);
            updateAllData();
            return;
        }
    }
}

void PlaylistModel::remove(const int &index)
{
    m_mediaElements.remove(index);
    updateAllData();
}

QUrl PlaylistModel::getSourceURL(const QModelIndex &index) const
{
    return m_mediaElements.value(index.row()).mediaPath();
}

QVariant PlaylistModel::getValueByKey(int index, QMediaMetaData::Key key) const
{
    return m_mediaElements.value(index).value(key);
}

QStringList PlaylistModel::getAllTitles() const
{
    QStringList titles;
    for(const QMediaMetaData &data : m_mediaElements)
    {
        titles << data.value(QMediaMetaData::Title).toString();
    }
    return titles;
}

void PlaylistModel::updateAllData()
{
    beginResetModel();
    endResetModel();
}

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
    return QMetaEnum::fromType<QMediaMetaData::Key>().keyCount(); // Because there is 28 types in QMediaMetaData::Key
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return m_mediaElements.at(index.row()).value((QMediaMetaData::Key) index.column());
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

