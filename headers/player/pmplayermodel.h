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

public:
    const QMediaPlayer *player() const;
    const QAudioOutput *audioOutput() const;
    PlaylistModel *currentPlaylist() const;
    void setCurrentPlaylist(PlaylistModel *newCurrentPlaylist);

public:
    void durationChanged(qint64);
    void positionChanged(qint64);
    void setToPosition(int);


    void openMedia(const QUrl& url);
    void stopMedia();
    void previousMedia();
    void playPauseMedia();
    void nextMedia();

    void muteMedia();
    void changeVolume(float);

    void changeSpeed(float);

public slots:
    void loadMedia(const QModelIndex& index);

private slots:
    void playerStatusUpdated(QMediaPlayer::MediaStatus);

private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    QTime m_currentMediaTime;
    QTime m_maxMediaTime;
    QUrl m_sourceFile;

    PlaylistModel *m_currentPlaylist;
};

#endif // PMPLAYERMODEL_H
