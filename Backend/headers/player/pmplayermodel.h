#ifndef PMPLAYERMODEL_H
#define PMPLAYERMODEL_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTime>
#include <QErrorMessage>
#include <QGraphicsVideoItem>
#include <QQueue>

#include "../playlist/playlistmodel.h"

class PMPlayerModel : public QObject
{
    Q_OBJECT
public:
    explicit PMPlayerModel(QObject *parent = nullptr);



signals:
    void playbackStateChanged(QMediaPlayer::PlaybackState);
    void playerChanged();
    void audioOutputChanged();

    void currentElementChanged();

    void currentIndexPlayingChanged();

public:
    const QMediaPlayer *player() const;
    const QAudioOutput *audioOutput() const;

    PlaylistModel *currentPlaylist() const;
    void setCurrentPlaylist(PlaylistModel *newCurrentPlaylist);

    PlaylistMediaElement currentElement() const;
    void setCurrentElement(const PlaylistMediaElement &newCurrentElement);

    QTime currentMediaTime() const;
    QTime maxMediaTime() const;

    void setVideoOutput(QGraphicsVideoItem *item);
    void setVideoOutput(QVideoWidget *widget);

    //----------------------------------------------------------------------------------------------------

    bool savePlaylistToFile(const QString &pathname) const;
    bool loadPlaylistFromFile(const QString &pathname) const;

    //----------------------------------------------------------------------------------------------------


    void durationChanged(qint64);
    void positionChanged(qint64);
    void setToPosition(int);

    void previousMedia();
    void nextMedia();
    void clearMedia();
    void openMedia(const QUrl& url);
    void stopMedia();
    void playPauseMedia();

    void muteMedia();
    void changeVolume(float);

    void changeSpeed(float);

    void playAsNext(const QModelIndex &index, bool replaceFirst = true);
    void addToQueue(const QModelIndex &index, bool isBack = true);
    void popQueue(bool isBack = true);
    bool removeFromQueue(const QModelIndex &index);
    void clearQueue();

    void insertMedia(const QList<QUrl> &urls, const QModelIndex &after);

    QModelIndex currentIndexPlaying() const;
    void setCurrentIndexPlaying(const QModelIndex &newCurrentIndexPlaying);

public slots:
    void loadMedia(const QModelIndex& index);
    void removeMedia(const QModelIndex& index);
    void removeMedia(const QList<QModelIndex>& indexes);

private slots:
    void playerStatusUpdated(QMediaPlayer::MediaStatus);
    void changeCurrentElement(const QUrl& source);

private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    QTime m_currentMediaTime;
    QTime m_maxMediaTime;
    PlaylistMediaElement m_currentElement;
    QModelIndex m_currentIndexPlaying;

    QQueue<QUrl> m_queue;

    PlaylistModel *m_currentPlaylist;
    Q_PROPERTY(PlaylistMediaElement currentElement READ currentElement WRITE setCurrentElement NOTIFY currentElementChanged)
    Q_PROPERTY(QModelIndex currentIndexPlaying READ currentIndexPlaying WRITE setCurrentIndexPlaying NOTIFY currentIndexPlayingChanged)
};

#endif // PMPLAYERMODEL_H
