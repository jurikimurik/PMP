#ifndef PLAYLISTMEDIAELEMENT_H
#define PLAYLISTMEDIAELEMENT_H

#include <QMediaMetaData>
#include <QUrl>

class PlaylistMediaElement : public QMediaMetaData
{
public:
    PlaylistMediaElement();
    PlaylistMediaElement(const QMediaMetaData &meta, const QUrl& urlSource);
    PlaylistMediaElement(const QMediaMetaData &meta, const QString& pathToFile);
    PlaylistMediaElement(const QUrl& urlSource);


    QUrl mediaPath() const;
    void setMediaPath(const QUrl &newMediaPath);
    bool isPathValid() const;

    friend bool operator ==(const PlaylistMediaElement &a, const PlaylistMediaElement &b);
    friend bool operator !=(const PlaylistMediaElement &a, const PlaylistMediaElement &b);

private:
    QUrl m_mediaPath;
};

inline bool operator ==(const PlaylistMediaElement &a, const PlaylistMediaElement &b)
{
    return ((QMediaMetaData) a == (QMediaMetaData) b) || (a.mediaPath() == b.mediaPath());
}
inline bool operator !=(const PlaylistMediaElement &a, const PlaylistMediaElement &b)
{
    return ((QMediaMetaData) a != (QMediaMetaData) b) || (a.mediaPath() != b.mediaPath());
}

#endif // PLAYLISTMEDIAELEMENT_H
