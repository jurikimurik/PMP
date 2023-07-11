#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QTableView>

class PlaylistView : public QTableView
{
    Q_OBJECT
public:
    PlaylistView(QWidget *parent = nullptr);

    // QAbstractItemView interface
public:
    virtual void setModel(QAbstractItemModel *model);
};

#endif // PLAYLISTVIEW_H
