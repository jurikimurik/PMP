#include "../../../headers/playlist/element/playlistmediaelement.h"

#include <QMediaPlayer>
#include <QDebug>

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

PlaylistMediaElement::PlaylistMediaElement(const QUrl &urlSource)
{
    //Trying to obtain metadata by using temporary player
    QMediaPlayer *tempPlayer = new QMediaPlayer();
    tempPlayer->setSource(urlSource);

    QMediaMetaData data = tempPlayer->metaData();
    tempPlayer->deleteLater();

    //Calling another constructor with new data
    new (this) PlaylistMediaElement(data, urlSource);
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
