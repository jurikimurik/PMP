#ifndef PLAYLISTMEDIAELEMENT_H
#define PLAYLISTMEDIAELEMENT_H

#include <QMediaMetaData>
#include <QUrl>

class PlaylistMediaElement : public QMediaMetaData
{
public:
    PlaylistMediaElement();
    PlaylistMediaElement(const QMediaMetaData& meta, const QUrl& urlSource);
    PlaylistMediaElement(const QMediaMetaData &meta, const QString& pathToFile);


    QUrl mediaPath() const;
    void setMediaPath(const QUrl &newMediaPath);
    bool isPathValid() const;

private:
    QUrl m_mediaPath;
};

#endif // PLAYLISTMEDIAELEMENT_H
