#ifndef PMPLAYERVIEW_H
#define PMPLAYERVIEW_H

#define DEBUG 1

#include <QMainWindow>
#include "pmplayermodel.h"

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
    void previousMedia();
    void playStopMedia();
    void nextMedia();
    void openFullscreen();
    void openMedia();
    void muteMedia();
    void changeVolume(int);
    void colorOptions();

private:
    Ui::PMPlayerView *ui;
    PMPlayerModel* m_model;

    void createConnections();
};
#endif // PMPLAYERVIEW_H
