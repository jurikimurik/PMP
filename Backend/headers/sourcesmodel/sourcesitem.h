#ifndef SOURCESITEM_H
#define SOURCESITEM_H

#include <QList>
#include <QVariant>
#include "../playlist/playlist.h"

class SourcesItem
{
public:
    explicit SourcesItem(const Playlist &data, SourcesItem *parentItem = nullptr);
    ~SourcesItem();

    void appendChild(SourcesItem *child);

    SourcesItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setData(const Playlist &playlist);
    int row() const;
    SourcesItem *parentItem();

    Playlist itemData() const;

private:
    QList<SourcesItem *> m_childItems;
    Playlist m_itemData;
    SourcesItem *m_parentItem;
};
#endif // SOURCESITEM_H
