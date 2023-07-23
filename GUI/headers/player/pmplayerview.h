#ifndef PMPLAYERVIEW_H
#define PMPLAYERVIEW_H

#include <QAction>
#include <QContextMenuEvent>
#include <QGraphicsVideoItem>
#include <QMainWindow>
#include <QSplitter>
#include <QTime>
#include <QSettings>
#include <QClipboard>
#include "GUI/headers/video/pmpvideowidget.h"
#include "../../../Backend/headers/player/pmplayermodel.h"
#include "../playlist/playlistview.h"

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
    void playerStatusUpdated(QMediaPlayer::MediaStatus status);
    void muteChanged(bool);

protected slots:
    void durationChanged(qint64);
    void positionChanged(qint64);
    void setToPosition(int);

    void savePlaylist();
    void loadPlaylist();

    void clearAllMedia();
    void removeMedia();
    void removeMedia(const QModelIndexList &indexes);
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

    void currentSelectionChanged(const QModelIndex &current);
    void currentElementChanged();
    void actionTriggered(QAction*);
private:
    QMenu *m_playlistMenu;
    QAction *addMediaAction;
    QAction *removeMediaAction;
    QAction *clearMediaAction;

    QAction *savePlaylistAction;
    QAction *loadPlaylistAction;

    QAction *playAsNextAction;
    QAction *addToQueueAction;
    QAction *removeFromQueue;

    QAction *copyMediaAction;
    QAction *cutMediaAction;
    QAction *insertHereAction;

    QErrorMessage *m_errorBox;

    Ui::PMPlayerView *ui;
    PMPlayerModel* m_model;
    PlaylistView *m_playlistView;
    QModelIndex m_currentIndex;

    PMPVideoWidget *m_videoWidget;
    QSplitter *m_splitter;

private:
    void createConnections();
    void readSettings();
    void writeSettings();
    bool areUserWantsToQuit();

    void copyToClipboard();
    void cutToClipboard();
    void pasteFromClipboard();

    // QWidget interface
protected:
    virtual void contextMenuEvent(QContextMenuEvent *event);

    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent *event);
};
#endif // PMPLAYERVIEW_H
