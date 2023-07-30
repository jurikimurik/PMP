#ifndef PLAYLIST_H
#define PLAYLIST_H

#include "element/playlistmediaelement.h"

class Playlist
{
public:
    Playlist(const QString &name = QString(),
             const QVector<PlaylistMediaElement> &data = QVector<PlaylistMediaElement>());

public:
    void add(const QUrl &url);
    void remove(const QUrl &url);
    void remove(const QList<QUrl> &urls);
    int positionOf(const PlaylistMediaElement &element) const;
    int count() const;

    PlaylistMediaElement get(const QUrl &source) const;
    PlaylistMediaElement get(const int &index) const;

    QString name() const;
    void setname(const QString &newPlaylistName);

    bool operator==(const Playlist &another) const {
        return m_playlistName == another.m_playlistName && m_mediaElements == another.m_mediaElements;
    }

protected:
    QVector<PlaylistMediaElement> m_mediaElements;
    QString m_playlistName;
};

#endif // PLAYLIST_H
