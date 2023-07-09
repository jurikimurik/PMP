#include "headers/player/pmplayerview.h"
#include "ui_pmplayerview.h"

#include <QGraphicsVideoItem>
#include <QMediaDevices>
#include <QAudioDevice>

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

    createConnections();

    connect(m_model->player(), &QMediaPlayer::mediaStatusChanged, this, &PMPlayerView::playerStatusUpdated);

    //TEST FILE
    m_model->player()->setSource(QUrl::fromLocalFile("/Users/urijmakovskij/Desktop/audio/05 156 - Just Chill.mp3"));
}

PMPlayerView::~PMPlayerView()
{
    delete ui;
}

void PMPlayerView::playerStatusUpdated(QMediaPlayer::MediaStatus status)
{
    switch(status)
    {
        case QMediaPlayer::NoMedia:

        break;

        case QMediaPlayer::LoadingMedia:

        break;

        case QMediaPlayer::LoadedMedia:

        break;

        case QMediaPlayer::StalledMedia:

        break;

        case QMediaPlayer::BufferingMedia:

        break;

        case QMediaPlayer::BufferedMedia:

        break;

        case QMediaPlayer::EndOfMedia:
        stopMedia();
        break;

        case QMediaPlayer::InvalidMedia:

        break;
    };

}

void PMPlayerView::durationChanged(qint64 duration)
{
#if DEBUG
    qDebug() << "PMPlayerView::durationChanged(qint64): " << duration;
#endif

    ui->timelineSlider->setMaximum(duration);
    m_maxMediaTime.setHMS(0, 0, 0);
    m_maxMediaTime = m_maxMediaTime.addMSecs(duration);
    ui->maxTimeLabel->setText(m_maxMediaTime.toString("hh:mm:ss"));
}

void PMPlayerView::positionChanged(qint64 position)
{
    ui->timelineSlider->setValue(position);
    m_currentMediaTime.setHMS(0, 0, 0);
    m_currentMediaTime = m_currentMediaTime.addMSecs(position);
    ui->currentTimeLabel->setText(m_currentMediaTime.toString("hh:mm:ss"));
}

void PMPlayerView::setToPosition(int position)
{
#if DEBUG
    qDebug() << "PMPlayerView::setToPosition(int): " << position;
#endif

    m_model->player()->setPosition(position);
}

void PMPlayerView::previousMedia()
{
#if DEBUG
    qDebug() << "PMPlayerView::previousMedia()";
#endif

}

void PMPlayerView::playPauseMedia()
{
    //If something is playing right now
    if(m_model->player()->isPlaying())
    {
        m_model->player()->pause();
    } else {
        //Otherwise
        m_model->player()->play();
    }

    ui->playButton->setChecked(m_model->player()->isPlaying());

#if DEBUG
    qDebug() << "PMPlayerView::playStopMedia():" << m_model->player()->isPlaying();
#endif
}

void PMPlayerView::nextMedia()
{

#if DEBUG
    qDebug() << "PMPlayerView::nextMedia()";
#endif

}

void PMPlayerView::openFullscreen()
{

#if DEBUG
    qDebug() << "PMPlayerView::openFullscreen()";
#endif

}

void PMPlayerView::openMedia()
{

#if DEBUG
    qDebug() << "PMPlayerView::openMedia()";
#endif

}

void PMPlayerView::stopMedia()
{
    //If something is playing right now
    if(m_model->player()->isPlaying())
    {
        m_model->player()->stop();
    }

    ui->playButton->setChecked(m_model->player()->isPlaying());

#if DEBUG
    qDebug() << "PMPlayerView::stopMedia():" << m_model->player()->isPlaying();
#endif
}

void PMPlayerView::muteMedia()
{

#if DEBUG
    qDebug() << "PMPlayerView::muteMedia()";
#endif

    if(m_model->audioOutput()->isMuted())
    {
        m_model->audioOutput()->setMuted(false);
    } else {
        m_model->audioOutput()->setMuted(true);
    }

    ui->volumeButton->setChecked(m_model->audioOutput()->isMuted());

}

void PMPlayerView::changeVolume(int)
{
    float volume = ui->volumeSlider->value() / 100.0;
    m_model->audioOutput()->setVolume(volume);

#if DEBUG
    qDebug() << "PMPlayerView::changeVolume(int): " << volume;
#endif
}

void PMPlayerView::changeSpeed(int index)
{

#if DEBUG
    qDebug() << "PMPlayerView::changeSpeed(int index): " << index;
#endif

    double speed = ui->speedBox->currentText().remove("x").toDouble();
    m_model->player()->setPlaybackRate(speed);
}

void PMPlayerView::colorOptions()
{

#if DEBUG
    qDebug() << "PMPlayerView::colorOptions()";
#endif

}


void PMPlayerView::createConnections()
{
    connect(m_model->player(), &QMediaPlayer::durationChanged, this, &PMPlayerView::durationChanged);
    connect(m_model->player(), &QMediaPlayer::positionChanged, this, &PMPlayerView::positionChanged);
    connect(ui->timelineSlider, &QSlider::sliderMoved, this, &PMPlayerView::setToPosition);

    connect(ui->stopButton, &QPushButton::clicked, this, &PMPlayerView::stopMedia);
    connect(ui->openButton, &QPushButton::clicked, this, &PMPlayerView::openMedia);
    connect(ui->backwardButton, &QPushButton::clicked, this, &PMPlayerView::previousMedia);
    connect(ui->playButton, &QPushButton::clicked, this, &PMPlayerView::playPauseMedia);
    connect(ui->forwardButton, &QPushButton::clicked, this, &PMPlayerView::nextMedia);
    connect(ui->fullscreenButton, &QPushButton::clicked, this, &PMPlayerView::openFullscreen);
    connect(ui->volumeButton, &QPushButton::clicked, this, &PMPlayerView::muteMedia);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &PMPlayerView::changeVolume);
    connect(ui->colorsButton, &QPushButton::clicked, this, &PMPlayerView::colorOptions);
}
