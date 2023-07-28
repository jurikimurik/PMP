#include "../../headers/sourcesmodel/sourcesmodel.h"

SourcesModel::SourcesModel(const QList<Playlist> &playlists, QObject *parent)
    : QAbstractItemModel{parent}, m_playlists{playlists}
{
    updateModelData();
}

SourcesModel::~SourcesModel()
{
    delete rootItem;
}

QModelIndex SourcesModel::parent(const QModelIndex &child) const
{
    if(!child.isValid())
        return QModelIndex();

    SourcesItem *childItem = static_cast<SourcesItem*>(child.internalPointer());
    SourcesItem *parentItem = childItem->parentItem();

    if(parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int SourcesModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid())
        return static_cast<SourcesItem*>(parent.internalPointer())->columnCount();
    return rootItem->columnCount();
}

void SourcesModel::remove(const Playlist &playlist)
{
    if(!m_playlists.contains(playlist))
        return;

    m_playlists.removeOne(playlist);
    updateModelData();
}

QModelIndex SourcesModel::indexOfPlaylist(const QString &name)
{
    for(int row = 0; row < rowCount(); ++row)
    {
        if(data(index(row, 0), Qt::DisplayRole).toString() == name)
            return index(row, 0);
    }

    return QModelIndex();
}

QModelIndex SourcesModel::indexOfPlaylist(const Playlist &playlist)
{
    return indexOfPlaylist(playlist.playlistName());
}

void SourcesModel::add(const Playlist &playlist)
{
    m_playlists.push_back(playlist);
    updateModelData();
}

void SourcesModel::changePlaylistName(const QModelIndex &whichPlaylist, const QString &toName)
{
    if(!hasIndex(whichPlaylist.row(), whichPlaylist.column(),whichPlaylist.parent()))
        return;

    SourcesItem* pointedItem = static_cast<SourcesItem*>(whichPlaylist.internalPointer());
    pointedItem->setData(0, toName);

    emit dataChanged(whichPlaylist, whichPlaylist);
}

void SourcesModel::updateModelData()
{
    if(rootItem)
        delete rootItem;

    rootItem = new SourcesItem({"Playlists"});

    for(const Playlist& playlist : m_playlists)
    {
        rootItem->appendChild(new SourcesItem({playlist.playlistName()}, rootItem));
    }

    emit dataChanged(index(0, 0),index(rowCount()-1, columnCount()-1));
}

QVariant SourcesModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(role != Qt::DisplayRole)
        return QVariant();

    SourcesItem *item = static_cast<SourcesItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags SourcesModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant SourcesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

int SourcesModel::rowCount(const QModelIndex &parent) const
{
    SourcesItem *parentItem;
    if(parent.column() > 0) {
        return 0;
    }

    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<SourcesItem*>(parent.internalPointer());

    return parentItem->childCount();
}

QModelIndex SourcesModel::index(int row, int column, const QModelIndex &parent) const
{
    if(!hasIndex(row, column, parent))
        return QModelIndex();

    SourcesItem *parentItem;

    if(!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<SourcesItem*>(parent.internalPointer());

    SourcesItem *childItem = parentItem->child(row);
    if(childItem)
        return createIndex(row, column, childItem);
    return QModelIndex();
}
