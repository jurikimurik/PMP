#include "../../headers/sourcesmodel/sourcesmodel.h"

SourcesModel::SourcesModel(QObject *parent)
    : QAbstractItemModel{parent}
{
    rootItem = new SourcesItem(Playlist(tr("Playlisty")));
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
    for(int i = 0; i < rootItem->childCount(); ++i)
    {
        if(playlist == rootItem->child(i)->itemData())
            delete rootItem->child(i);
    }

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
    rootItem->appendChild(new SourcesItem(playlist, rootItem));
    updateModelData();
}

void SourcesModel::changePlaylistName(const QModelIndex &whichPlaylist, const QString &toName)
{
    if(!hasIndex(whichPlaylist.row(), whichPlaylist.column(),whichPlaylist.parent()))
        return;

    SourcesItem* pointedItem = static_cast<SourcesItem*>(whichPlaylist.internalPointer());

    Playlist newPlaylist = pointedItem->itemData();
    newPlaylist.setPlaylistName(toName);

    pointedItem->setData(newPlaylist);

    emit dataChanged(whichPlaylist, whichPlaylist);
}

void SourcesModel::playlistChanged(const Playlist &playlist)
{
    QModelIndex index = indexOfPlaylist(playlist);

    if(!index.isValid())
        return;

    SourcesItem *item = static_cast<SourcesItem*>(index.internalPointer());
    item->setData(playlist);

    updateModelData();
}

void SourcesModel::updateModelData()
{
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
