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
    void remove(const QUrl &url);
    void remove(const QList<QUrl> &urls);
    void insert(const QUrl &url, const QModelIndex &after);
    void insert(const QList<QUrl> &urls, const QModelIndex &after);
    int count() const;
    int positionOf(const PlaylistMediaElement &element) const;
    QModelIndex indexPositionOf(const PlaylistMediaElement &element) const;

    PlaylistMediaElement get(const QUrl &source) const;
    PlaylistMediaElement get(const int &index) const;

    QUrl getSourceURL(const QModelIndex &index) const;
    QStringList getAllInfoOfKey(const QMediaMetaData::Key &key) const;
    bool saveToFile(const QString &pathname);
    bool loadFromFile(const QString &pathname);

private:
    void updateAllData();
    bool saveToM3UFile(const QString &pathname);
    bool loadFromM3UFile(const QString &pathname);

    void loadM3UInfoInto(const QString &line, PlaylistMediaElement& intoElement);

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
