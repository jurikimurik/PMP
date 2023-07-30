#include "../../headers/player/pmplayermodel.h"

#include <QErrorMessage>
#include <QFileDialog>

void PMPlayerModel::connectPlaylistToSourcesModel()
{
    connect(m_currentPlaylist, &PlaylistModel::nameChanged, this, &PMPlayerModel::updateCurrentPlaylistName);
    connect(m_currentPlaylist, &PlaylistModel::mediaChanged, m_sourcesModel, &SourcesModel::playlistChanged);
}

PMPlayerModel::PMPlayerModel(QObject *parent)
    : QObject{parent}
{
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    m_currentPlaylist = new PlaylistModel(this);
    m_sourcesModel = new SourcesModel(this);
    m_sourcesModel->add(*m_currentPlaylist);
    m_currentPlaylistPlaying = m_sourcesModel->index(0, 0);

    connect(m_player, &QMediaPlayer::sourceChanged, this, &PMPlayerModel::changeCurrentElement);

    connectPlaylistToSourcesModel();
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

void PMPlayerModel::previousMedia()
{
    loadMedia(m_currentIndexPlaying.siblingAtRow(m_currentIndexPlaying.row()-1));
}

void PMPlayerModel::nextMedia()
{
    if(m_queue.isEmpty()) {
        loadMedia(m_currentIndexPlaying.siblingAtRow(m_currentIndexPlaying.row()+1));
    }
    else {
        QPair<QModelIndex, QUrl> mediaPair = m_queue.dequeue();
        if(m_currentPlaylist->getSourceURL(mediaPair.first) == mediaPair.second)
            //If QModelIndex is still pointing at good QUrl, play this
            loadMedia(mediaPair.first);
        else {
            //If QModelIndex pointing on some another QUrl - skip
            nextMedia();
        }

    }

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
    if(index.isValid() && index.row() < m_currentPlaylist->count()) {
        setCurrentIndexPlaying(index);
    }

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

void PMPlayerModel::playAsNext(const QModelIndex &index, bool replaceFirst)
{
    qDebug() << index.row() << index.column();
    if(replaceFirst && !m_queue.isEmpty())
        m_queue.pop_front();

    addToQueue(index, false);
}

void PMPlayerModel::addToQueue(const QModelIndex &index, bool isBack)
{
    QUrl url = m_currentPlaylist->getSourceURL(index);
    if(isBack)
        m_queue.push_back(QPair<QModelIndex, QUrl>(index, url));
    else
        m_queue.push_front(QPair<QModelIndex, QUrl>(index,url));
}

void PMPlayerModel::popQueue(bool isBack)
{
    if(isBack && !m_queue.isEmpty())
        m_queue.pop_back();
    else
        m_queue.pop_front();
}

bool PMPlayerModel::removeFromQueue(const QModelIndex &index)
{
    QUrl url = m_currentPlaylist->getSourceURL(index);
    int mediaIndex = m_queue.indexOf(QPair<QModelIndex, QUrl>(index, url));
    if(mediaIndex != -1) {
        m_queue.remove(mediaIndex);
        //Founded.
        return true;
    }
    return false;
}

void PMPlayerModel::clearQueue()
{
    m_queue.clear();
}

void PMPlayerModel::insertMedia(const QList<QUrl> &urls, const QModelIndex &after)
{
    m_currentPlaylist->insert(urls, after);
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

void PMPlayerModel::updateCurrentPlaylistName(const QString &newName)
{
    m_sourcesModel->changePlaylistName(m_currentPlaylistPlaying, newName);
}

SourcesModel *PMPlayerModel::sourcesModel() const
{
    return m_sourcesModel;
}

QModelIndex PMPlayerModel::currentIndexPlaying() const
{
    return m_currentIndexPlaying;
}

void PMPlayerModel::setCurrentIndexPlaying(const QModelIndex &newCurrentIndexPlaying)
{
    if (m_currentIndexPlaying == newCurrentIndexPlaying)
        return;
    m_currentIndexPlaying = newCurrentIndexPlaying;
    emit currentIndexPlayingChanged();
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
    if(m_currentPlaylist->loadFromFile(pathname))
    {
        return true;
    } else
        return false;
}

QTime PMPlayerModel::currentMediaTime() const
{
    return m_currentMediaTime;
}

