#ifndef PMPLAYERMODEL_H
#define PMPLAYERMODEL_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>


class PMPlayerModel : public QObject
{
    Q_OBJECT
public:
    explicit PMPlayerModel(QObject *parent = nullptr);

    QMediaPlayer *player() const;
    void setPlayer(QMediaPlayer *newPlayer);
    QAudioOutput *audioOutput() const;
    void setAudioOutput(QAudioOutput *newAudioOutput);

signals:

    void playerChanged();
    void audioOutputChanged();

private:
    QMediaPlayer* m_player;
    QAudioOutput* m_audioOutput;

    Q_PROPERTY(QMediaPlayer *player READ player WRITE setPlayer NOTIFY playerChanged)
    Q_PROPERTY(QAudioOutput *audioOutput READ audioOutput WRITE setAudioOutput NOTIFY audioOutputChanged)
};

#endif // PMPLAYERMODEL_H
