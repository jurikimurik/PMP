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

    QUrl getSourceURL(int index) const;

    QVariant getValueByKey(int index, QMediaMetaData::Key key) const;

    QStringList getAllTitles() const;


private:
    QVector<QMediaMetaData> m_values;
};

#endif // PLAYLISTMODEL_H
