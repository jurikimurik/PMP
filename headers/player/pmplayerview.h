#ifndef PMPLAYERVIEW_H
#define PMPLAYERVIEW_H

#include <QMainWindow>
#include <QTime>
#include "pmplayermodel.h"

#define DEBUG 1

QT_BEGIN_NAMESPACE
namespace Ui { class PMPlayerView; }
QT_END_NAMESPACE

class PMPlayerView : public QMainWindow
{
    Q_OBJECT

public:
    PMPlayerView(QWidget *parent = nullptr, PMPlayerModel *model = nullptr);
    ~PMPlayerView();

    //Slots connected with GUI (they call functions from model as well)
public slots:
    void playbackStateChanged(QMediaPlayer::PlaybackState);
    void muteChanged(bool);

protected slots:
    void durationChanged(qint64);
    void positionChanged(qint64);
    void setToPosition(int);

    void openMedia();
    void stopMedia();
    void previousMedia();
    void playPauseMedia();
    void nextMedia();

    void muteMedia();
    void changeVolume(int);

    void changeSpeed();

    void openFullscreen();
    void colorOptions();

private:
    Ui::PMPlayerView *ui;
    PMPlayerModel* m_model;

    void createConnections();
};
#endif // PMPLAYERVIEW_H
