#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H


#include <QAbstractItemModel>
#include <QMediaMetaData>
#include <QMediaPlayer>
#include "element/playlistmediaelement.h"
#include "playlist.h"

class PlaylistModel : public QAbstractItemModel, public Playlist
{
    Q_OBJECT
public:
    explicit PlaylistModel(QObject *parent = nullptr);

    //Reimplemented functions from Playlist (for updateAllData() method)
    void add(const QUrl &url);
    void remove(const QUrl &url);
    void remove(const QList<QUrl> &urls);

    void insert(const QUrl &url, const QModelIndex &after);
    void insert(const QList<QUrl> &urls, const QModelIndex &after);

    QModelIndex indexPositionOf(const PlaylistMediaElement &element) const;

    QUrl getSourceURL(const QModelIndex &index) const;
    bool saveToFile(const QString &pathname);
    bool loadFromFile(const QString &pathname);

signals:
    void nameChanged(const QString &newPLaylistName);
    void mediaChanged(const Playlist &newPlaylist);

public slots:
    void setName(const QString &newPlaylistName);

private:
    void updateAllData();
    bool saveToM3UFile(const QString &pathname);
    bool loadFromM3UFile(const QString &pathname);

    void loadM3UInfoInto(const QString &line, PlaylistMediaElement& intoElement);

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
