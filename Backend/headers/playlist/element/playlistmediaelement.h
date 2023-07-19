#ifndef PLAYLISTMEDIAELEMENT_H
#define PLAYLISTMEDIAELEMENT_H

#include <QMediaMetaData>
#include <QUrl>

//Basically represents one media in playlist. It combines meta data and url providing to file.
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

    void loadMetaData();
    void checkMetaData();
};

//Due to a bug [QTBUG-76614] in QT framework, this additional comparation methods have been added.
inline bool operator ==(const PlaylistMediaElement &a, const PlaylistMediaElement &b)
{
    return ((QMediaMetaData) a == (QMediaMetaData) b) || (a.mediaPath() == b.mediaPath());
}
inline bool operator !=(const PlaylistMediaElement &a, const PlaylistMediaElement &b)
{
    return ((QMediaMetaData) a != (QMediaMetaData) b) || (a.mediaPath() != b.mediaPath());
}

#endif // PLAYLISTMEDIAELEMENT_H
