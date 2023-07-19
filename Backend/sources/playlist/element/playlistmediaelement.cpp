#include "../../../headers/playlist/element/playlistmediaelement.h"

#include <QMediaPlayer>
#include <QDebug>
#include <QFileInfo>

PlaylistMediaElement::PlaylistMediaElement()
{

}

PlaylistMediaElement::PlaylistMediaElement(const QMediaMetaData &meta, const QUrl &urlSource)
    : QMediaMetaData(meta), m_mediaPath(urlSource)

{
    checkMetaData();
}

PlaylistMediaElement::PlaylistMediaElement(const QMediaMetaData &meta, const QString &pathToFile)
    : QMediaMetaData(meta), m_mediaPath(QUrl::fromLocalFile(pathToFile))
{
    new (this) PlaylistMediaElement(meta, QUrl::fromLocalFile(pathToFile));
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
    loadMetaData();
}

bool PlaylistMediaElement::isPathValid() const
{
    return m_mediaPath.isValid();
}

void PlaylistMediaElement::loadMetaData()
{
    new (this) PlaylistMediaElement(m_mediaPath);
}

void PlaylistMediaElement::checkMetaData()
{
    if(stringValue(QMediaMetaData::Title).isEmpty())
    {
        QFileInfo info(m_mediaPath.toLocalFile());
        insert(QMediaMetaData::Title, info.fileName());
    }
}
