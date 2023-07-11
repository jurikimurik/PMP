#include "playlistview.h"

#include <QHeaderView>

PlaylistView::PlaylistView(QWidget *parent) : QTableView(parent)
{}

void PlaylistView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    for(int i = 1; i < model->columnCount(); ++i)
    {
        hideColumn(i);
    }
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
