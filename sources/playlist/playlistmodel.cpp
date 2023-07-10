#include "../../headers/playlist/playlistmodel.h"

PlaylistModel::PlaylistModel(QObject *parent) : QAbstractItemModel(parent)
{}

/*void PlaylistModel::add(const QUrl &url)
{
    QMediaPlayer* tempPlayer = new QMediaPlayer(this);
    tempPlayer->setSource(url);

    QMediaMetaData metaData = tempPlayer->metaData();
    if(!m_values.contains(metaData) &&
        (tempPlayer->error() == QMediaPlayer::NoError || tempPlayer->error() == QMediaPlayer::FormatError))
    {
        m_values.push_back(tempPlayer->metaData());
    }
}

void PlaylistModel::remove(const QString &title)
{
    for(int i = 0; i < m_values.size(); ++i)
    {
        QMediaMetaData &data = m_values[i];

        if(data.value(QMediaMetaData::Title).toString() == title)
        {
            m_values.remove(i);
            return;
        }
    }
}

void PlaylistModel::remove(const QUrl &url)
{
    for(int i = 0; i < m_values.size(); ++i)
    {
        QMediaMetaData &data = m_values[i];

        if(data.value(QMediaMetaData::Url).toUrl() == url)
        {
            m_values.remove(i);
            return;
        }
    }
}

void PlaylistModel::remove(const int &index)
{
    m_values.remove(index);
}

QUrl PlaylistModel::getSourceURL(int index) const
{
    return m_values.value(index).value(QMediaMetaData::Url).toUrl();
}

QVariant PlaylistModel::getValueByKey(int index, QMediaMetaData::Key key) const
{
    return m_values.value(index).value(key);
}

QStringList PlaylistModel::getAllTitles() const
{
    QStringList titles;
    for(const QMediaMetaData &data : m_values)
    {
        titles << data.value(QMediaMetaData::Title).toString();
    }
    return titles;
}*/

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
    return m_values.count();
}

int PlaylistModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 28; // Because there is 28 types in QMediaMetaData::Key
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return m_values.at(index.row()).value((QMediaMetaData::Key) index.column());
    }

    return QVariant();
}
