#include "../../headers/playlist/playlistmodel.h"

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractItemModel(parent)
{}

void PlaylistModel::add(const QUrl &url)
{
    QMediaPlayer* tempPlayer = new QMediaPlayer(this);
    tempPlayer->setSource(url);

    QMediaMetaData metaData = tempPlayer->metaData();
    if(!m_urlPathes.contains(url) && (tempPlayer->error() == QMediaPlayer::NoError ||
                                       tempPlayer->error() == QMediaPlayer::FormatError))
    {
        m_metaDatas.push_back(metaData);
        m_urlPathes.push_back(tempPlayer->source());
        updateAllData();
    }
}

void PlaylistModel::remove(const QString &title)
{
    for(int i = 0; i < m_metaDatas.size(); ++i)
    {
        QMediaMetaData &data = m_metaDatas[i];

        if(data.value(QMediaMetaData::Title).toString() == title)
        {
            m_metaDatas.remove(i);
            m_urlPathes.remove(i);
            updateAllData();
            return;
        }
    }
}

void PlaylistModel::remove(const QUrl &url)
{
    for(int i = 0; i < m_metaDatas.size(); ++i)
    {
        QMediaMetaData &data = m_metaDatas[i];

        if(data.value(QMediaMetaData::Url).toUrl() == url)
        {
            m_metaDatas.remove(i);
            m_urlPathes.remove(i);
            updateAllData();
            return;
        }
    }
}

void PlaylistModel::remove(const int &index)
{
    //Url first! (Because of &)
    m_urlPathes.remove(index);
    m_metaDatas.remove(index);
    updateAllData();
}

QUrl PlaylistModel::getSourceURL(int index) const
{
    return m_urlPathes.value(index);
}

QVariant PlaylistModel::getValueByKey(int index, QMediaMetaData::Key key) const
{
    return m_metaDatas.value(index).value(key);
}

QStringList PlaylistModel::getAllTitles() const
{
    QStringList titles;
    for(const QMediaMetaData &data : m_metaDatas)
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
    return m_metaDatas.count();
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
        return m_metaDatas.at(index.row()).value((QMediaMetaData::Key) index.column());
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

