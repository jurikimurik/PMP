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

    QString playlistName() const;
    void setPlaylistName(const QString &newPlaylistName);

protected:
    QVector<PlaylistMediaElement> m_mediaElements;
    QString m_playlistName;
};

#endif // PLAYLIST_H