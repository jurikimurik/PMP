#include "../../headers/player/pmplayermodel.h"

PMPlayerModel::PMPlayerModel(QObject *parent)
    : QObject{parent}
{
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    m_currentPlaylist = new PlaylistModel(this);
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

void PMPlayerModel::loadMedia(const QModelIndex &index)
{
    stopMedia();
    m_player->setSource(m_currentPlaylist->getSourceURL(index));
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

void PMPlayerModel::previousMedia()
{

}

void PMPlayerModel::playPauseMedia()
{
    //If something is playing right now
    if(m_player->isPlaying())
    {
        m_player->pause();
    } else {
        //Otherwise
        m_player->play();
    }
}

void PMPlayerModel::nextMedia()
{

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
        stopMedia();
        break;

    case QMediaPlayer::InvalidMedia:

        break;
    };
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

QTime PMPlayerModel::currentMediaTime() const
{
    return m_currentMediaTime;
}

