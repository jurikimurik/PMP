#ifndef PMPLAYERVIEW_H
#define PMPLAYERVIEW_H

#include <QMainWindow>
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

#if DEBUG
public slots:
    void statusChanged();
#endif

protected slots:
    void durationChanged(qint64);
    void setToPosition(int);

    void openMedia();
    void previousMedia();
    void playStopMedia();
    void nextMedia();

    void muteMedia();
    void changeVolume(int);

    void openFullscreen();
    void colorOptions();

private:
    Ui::PMPlayerView *ui;
    PMPlayerModel* m_model;

    void createConnections();
};
#endif // PMPLAYERVIEW_H
