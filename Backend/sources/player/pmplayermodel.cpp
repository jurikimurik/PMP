#include "../../headers/player/pmplayermodel.h"

#include <QErrorMessage>
#include <QFileDialog>

PMPlayerModel::PMPlayerModel(QObject *parent)
    : QObject{parent}
{
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    m_currentPlaylist = new PlaylistModel(this);
    connect(m_player, &QMediaPlayer::sourceChanged, this, &PMPlayerModel::changeCurrentElement);
}

void PMPlayerModel::durationChanged(qint64 duration)
{
    m_maxMediaTime.setHMS(0, 0, 0);
    m_maxMediaTime = m_maxMediaTime.addMSecs(duration);
}

void PMPlayerModel::positionChanged(qint64 position)
{
    m_currentMediaTime.setHMS(0, 0, 0);
    m_currentMediaTime = m_currentMediaTime.addMSecs(position);
}

void PMPlayerModel::setToPosition(int position)
{
    m_player->setPosition(position);
}

void PMPlayerModel::clearMedia()
{
    QModelIndexList indexes;
    for(int i = 0; i < m_currentPlaylist->rowCount(QModelIndex()); ++i)
        indexes << m_currentPlaylist->index(i, 0, QModelIndex());

    if(!indexes.isEmpty())
        removeMedia(indexes);
}

void PMPlayerModel::removeMedia(const QModelIndex &index)
{
    QUrl url = m_currentPlaylist->getSourceURL(index);
    if(url == m_player->source()) {
        stopMedia();
        m_player->setSource(QUrl());
    }

    if(url.isValid()) {
        m_currentPlaylist->remove(url);
        if(m_player->source() == QUrl())
            loadMedia(index);
    }

}

void PMPlayerModel::removeMedia(const QList<QModelIndex> &indexes)
{
    QList<QUrl> urls;
    for(const QModelIndex &index : indexes) {
        QUrl url = m_currentPlaylist->getSourceURL(index);
        urls.push_back(url);

        if(url == m_player->source()) {
            stopMedia();
            m_player->setSource(QUrl());
        }
    }

    m_currentPlaylist->remove(urls);
    if(m_player->source() == QUrl())
        loadMedia(indexes.last());
}

void PMPlayerModel::loadMedia(const QModelIndex &index)
{
    stopMedia();
    QUrl url = m_currentPlaylist->getSourceURL(index);
    if(url.isValid())
        m_player->setSource(url);
}

void PMPlayerModel::openMedia(const QUrl &url)
{
    m_currentPlaylist->add(url);
}

void PMPlayerModel::stopMedia()
{
    //If something is playing right now
    if(m_player->isPlaying())
    {
        m_player->stop();
    }

    emit playbackStateChanged(m_player->playbackState());
}

void PMPlayerModel::playPauseMedia()
{
    if(m_player->source().isEmpty())
        loadMedia(m_currentPlaylist->index(0, 0, QModelIndex()));

    //If something is playing right now
    if(m_player->isPlaying())
    {
        m_player->pause();
    } else {
        //Otherwise
        m_player->play();
    }
}

void PMPlayerModel::muteMedia()
{
    if(m_audioOutput->isMuted())
    {
        m_audioOutput->setMuted(false);
    } else {
        m_audioOutput->setMuted(true);
    }
}

void PMPlayerModel::changeVolume(float value)
{
    m_audioOutput->setVolume(value);
}

void PMPlayerModel::changeSpeed(float speed)
{
    m_player->setPlaybackRate(speed);
}

void PMPlayerModel::playerStatusUpdated(QMediaPlayer::MediaStatus status)
{
    switch(status)
    {
    case QMediaPlayer::NoMedia:
        stopMedia();
        break;

    case QMediaPlayer::LoadingMedia:

        break;

    case QMediaPlayer::LoadedMedia:

        break;

    case QMediaPlayer::StalledMedia:

        break;

    case QMediaPlayer::BufferingMedia:

        break;

    case QMediaPlayer::BufferedMedia:

        break;

    case QMediaPlayer::EndOfMedia:
        break;

    case QMediaPlayer::InvalidMedia:
        stopMedia();
        break;
    };
}

void PMPlayerModel::changeCurrentElement(const QUrl &source)
{
    setCurrentElement(m_currentPlaylist->get(source));
}

PlaylistMediaElement PMPlayerModel::currentElement() const
{
    return m_currentElement;
}

void PMPlayerModel::setCurrentElement(const PlaylistMediaElement &newCurrentElement)
{
    if (m_currentElement == newCurrentElement)
        return;
    m_currentElement = newCurrentElement;
    emit currentElementChanged();
}

PlaylistModel *PMPlayerModel::currentPlaylist() const
{
    return m_currentPlaylist;
}

void PMPlayerModel::setCurrentPlaylist(PlaylistModel *newCurrentPlaylist)
{
    m_currentPlaylist = newCurrentPlaylist;
}

const QAudioOutput *PMPlayerModel::audioOutput() const
{
    return m_audioOutput;
}

const QMediaPlayer *PMPlayerModel::player() const
{
    return m_player;
}

QTime PMPlayerModel::maxMediaTime() const
{
    return m_maxMediaTime;
}

void PMPlayerModel::setVideoOutput(QGraphicsVideoItem *item)
{
    m_player->setVideoOutput(item);
}

void PMPlayerModel::setVideoOutput(QVideoWidget *widget)
{
    m_player->setVideoOutput(widget);
}

bool PMPlayerModel::savePlaylistToFile(const QString& pathname) const
{
    return m_currentPlaylist->saveToFile(pathname);
}

bool PMPlayerModel::loadPlaylistFromFile(const QString& pathname) const
{
    return m_currentPlaylist->loadFromFile(pathname);
}

QTime PMPlayerModel::currentMediaTime() const
{
    return m_currentMediaTime;
}

