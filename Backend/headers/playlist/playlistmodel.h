#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractItemModel>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include "element/playlistmediaelement.h"

class PlaylistModel : public QAbstractItemModel
{
signals:
    void playlistChanged(QStringList);

public:
    PlaylistModel(QObject *parent = nullptr);

    void add(const QUrl &url);

    void remove(const QString &title);
    void remove(const QUrl &url);
    void remove(const int &index);

    PlaylistMediaElement get(const QUrl &source);

    QVariant getValueByKey(int index, QMediaMetaData::Key key) const;

    QUrl getSourceURL(const QModelIndex &index) const;
    QStringList getAllTitles() const;

private:
    void updateAllData();

private:
    QVector<PlaylistMediaElement> m_mediaElements;

    // QAbstractItemModel interface
public:
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;

    // QAbstractItemModel interface
public:
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;
};

#endif // PLAYLISTMODEL_H
