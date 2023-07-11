#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QAbstractItemModel>
#include <QMediaMetaData>
#include <QMediaPlayer>

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

    QVariant getValueByKey(int index, QMediaMetaData::Key key) const;

    QUrl getSourceURL(int index) const;
    QStringList getAllTitles() const;

private:
    void dataChanged();

private:
    QVector<QMediaMetaData> m_metaDatas;
    QVector<QUrl> m_urlPathes;

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
