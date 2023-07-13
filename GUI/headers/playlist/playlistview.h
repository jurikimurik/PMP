#ifndef PLAYLISTVIEW_H
#define PLAYLISTVIEW_H

#include <QTableView>

class PlaylistView : public QTableView
{
    Q_OBJECT
public:
    PlaylistView(QWidget *parent = nullptr);

public slots:
    void updateSelection();

    // QAbstractItemView interface
public:
    virtual void setModel(QAbstractItemModel *model);

    // QAbstractItemView interface
protected slots:
    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};

#endif // PLAYLISTVIEW_H
