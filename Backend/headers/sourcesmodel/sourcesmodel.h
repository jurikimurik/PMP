#ifndef SOURCESMODEL_H
#define SOURCESMODEL_H

#include <QAbstractItemModel>
#include "../playlist/playlist.h"
#include "sourcesitem.h"

class SourcesModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit SourcesModel(const QList<Playlist> &playlists = QList<Playlist>(), QObject *parent = nullptr);
    ~SourcesModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    SourcesItem* rootItem;

private:
    void setupModelData();

public:
    QVector<Playlist> m_playlists;
};

#endif // SOURCESMODEL_H
