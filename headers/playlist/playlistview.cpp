#include "playlistview.h"

#include <QHeaderView>

PlaylistView::PlaylistView(QWidget *parent) : QTableView(parent)
{
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::ExtendedSelection);

    //Method updateSelection was added because of strange bug when nothing being selected even after dataChanged signal.
    connect(this, &PlaylistView::clicked, this, &PlaylistView::updateSelection);
    connect(this, &PlaylistView::activated, this, &PlaylistView::updateSelection);
    connect(this, &PlaylistView::entered, this, &PlaylistView::updateSelection);
    connect(this, &PlaylistView::doubleClicked, this, &PlaylistView::updateSelection);
    connect(this, &PlaylistView::pressed, this, &PlaylistView::updateSelection);
    connect(this, &PlaylistView::viewportEntered, this, &PlaylistView::updateSelection);
}

void PlaylistView::updateSelection()
{
    viewport()->repaint();
}

void PlaylistView::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    for(int i = 1; i < model->columnCount(); ++i)
    {
        hideColumn(i);
    }
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
