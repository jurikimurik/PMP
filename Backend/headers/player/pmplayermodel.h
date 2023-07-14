#ifndef PMPLAYERMODEL_H
#define PMPLAYERMODEL_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTime>

#include "../playlist/playlistmodel.h"

class PMPlayerModel : public QObject
{
    Q_OBJECT
public:
    explicit PMPlayerModel(QObject *parent = nullptr);

    QTime currentMediaTime() const;

    QTime maxMediaTime() const;

signals:
    void playbackStateChanged(QMediaPlayer::PlaybackState);
    void playerChanged();
    void audioOutputChanged();

    void currentElementChanged();

public:
    const QMediaPlayer *player() const;
    const QAudioOutput *audioOutput() const;
    PlaylistModel *currentPlaylist() const;
    void setCurrentPlaylist(PlaylistModel *newCurrentPlaylist);
    PlaylistMediaElement currentElement() const;
    void setCurrentElement(const PlaylistMediaElement &newCurrentElement);

public:
    void durationChanged(qint64);
    void positionChanged(qint64);
    void setToPosition(int);


    void openMedia(const QUrl& url);
    void stopMedia();
    void playPauseMedia();

    void muteMedia();
    void changeVolume(float);

    void changeSpeed(float);

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

    PlaylistModel *m_currentPlaylist;
    Q_PROPERTY(PlaylistMediaElement currentElement READ currentElement WRITE setCurrentElement NOTIFY currentElementChanged)
};

#endif // PMPLAYERMODEL_H