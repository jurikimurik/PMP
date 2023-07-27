#include "../../headers/sourcesmodel/sourcesitem.h"

SourcesItem::SourcesItem(const QList<QVariant> &data, SourcesItem *parentItem)
    : m_itemData(data), m_parentItem(parentItem)
{

}

SourcesItem::~SourcesItem()
{
    qDeleteAll(m_childItems);
}

void SourcesItem::appendChild(SourcesItem *child)
{
    m_childItems.append(child);
}

SourcesItem *SourcesItem::child(int row)
{
    if(row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int SourcesItem::childCount() const
{
    return m_childItems.count();
}

int SourcesItem::columnCount() const
{
    return m_itemData.count();
}

QVariant SourcesItem::data(int column) const
{
    if(column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

int SourcesItem::row() const
{
    if(m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<SourcesItem*>(this));

    return 0;
}

SourcesItem *SourcesItem::parentItem()
{
    return m_parentItem;
}

