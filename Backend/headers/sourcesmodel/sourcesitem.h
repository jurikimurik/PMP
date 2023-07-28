#ifndef SOURCESITEM_H
#define SOURCESITEM_H

#include <QList>
#include <QVariant>

class SourcesItem
{
public:
    explicit SourcesItem(const QList<QVariant> &data, SourcesItem *parentItem = nullptr);
    ~SourcesItem();

    void appendChild(SourcesItem *child);

    SourcesItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setData(int column, QVariant data);
    int row() const;
    SourcesItem *parentItem();

private:
    QList<SourcesItem *> m_childItems;
    QList<QVariant> m_itemData;
    SourcesItem *m_parentItem;
};
#endif // SOURCESITEM_H
