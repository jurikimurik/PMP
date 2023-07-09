#include "headers/player/pmplayerview.h"
#include "ui_pmplayerview.h"

#include <QGraphicsVideoItem>
#include <QMediaDevices>
#include <QAudioDevice>
PMPlayerView::~PMPlayerView()
{
    delete ui;
}

void PMPlayerView::statusChanged()
{
    qDebug() << m_model->player()->mediaStatus();
    qDebug() << m_model->player()->playbackState();

}


PMPlayerView::PMPlayerView(QWidget *parent, PMPlayerModel *model)
    : QMainWindow(parent)
    , ui(new Ui::PMPlayerView)
    , m_model(model)
{
    ui->setupUi(this);
    //If there is no model created or connected
    //  - create a new one.
    if(m_model == nullptr)
        m_model = new PMPlayerModel(this);


    connect(m_model->player(), &QMediaPlayer::mediaStatusChanged, this, &PMPlayerView::statusChanged);

    //SIMPLE TEST
    m_model->player()->setSource(QUrl::fromLocalFile("/Users/urijmakovskij/Desktop/audio/05 156 - Just Chill.mp3"));
    m_model->player()->play();
}
