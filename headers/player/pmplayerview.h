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

public slots:
    void playerStatusUpdated(QMediaPlayer::MediaStatus);

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

    void changeSpeed(int index);

    void openFullscreen();
    void colorOptions();

private:
    Ui::PMPlayerView *ui;
    PMPlayerModel* m_model;
    QTime m_currentMediaTime;
    QTime m_maxMediaTime;

    void createConnections();
};
#endif // PMPLAYERVIEW_H
