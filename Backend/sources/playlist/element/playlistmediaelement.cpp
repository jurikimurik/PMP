#include "../../../headers/playlist/element/playlistmediaelement.h"

PlaylistMediaElement::PlaylistMediaElement()
{

}

PlaylistMediaElement::PlaylistMediaElement(const QMediaMetaData &meta, const QUrl &urlSource)
    : QMediaMetaData(meta), m_mediaPath(urlSource)

{

}

PlaylistMediaElement::PlaylistMediaElement(const QMediaMetaData &meta, const QString &pathToFile)
    : QMediaMetaData(meta), m_mediaPath(QUrl::fromLocalFile(pathToFile))
{

}

QUrl PlaylistMediaElement::mediaPath() const
{
    return m_mediaPath;
}

void PlaylistMediaElement::setMediaPath(const QUrl &newMediaPath)
{
    m_mediaPath = newMediaPath;
}

bool PlaylistMediaElement::isPathValid() const
{
    return m_mediaPath.isValid();
}
