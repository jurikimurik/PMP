#ifndef PMPLAYERMODEL_H
#define PMPLAYERMODEL_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QTime>


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

private slots:
    void playerStatusUpdated(QMediaPlayer::MediaStatus);

private:
    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    QTime m_currentMediaTime;
    QTime m_maxMediaTime;
    QUrl m_sourceFile;
};

#endif // PMPLAYERMODEL_H
