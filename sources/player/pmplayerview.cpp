#include "headers/player/pmplayerview.h"
#include "ui_pmplayerview.h"

#include <QGraphicsVideoItem>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QFileDialog>

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

    m_playlistView = new PlaylistView(this);
    ui->mediaWidget->layout()->addWidget(m_playlistView);
    m_playlistView->setModel(m_model->currentPlaylist());

    m_playlistMenu = new QMenu(m_playlistView);
    addMediaAction = new QAction(tr("Dodaj media"), m_playlistMenu);
    removeMediaAction = new QAction(tr("Usun media"), m_playlistMenu);
    m_playlistMenu->addActions({addMediaAction, removeMediaAction});

    createConnections();
    durationChanged(m_model->player()->duration());
}

PMPlayerView::~PMPlayerView()
{
    delete ui;
}

void PMPlayerView::playbackStateChanged(QMediaPlayer::PlaybackState state)
{
    switch(state) {
    case QMediaPlayer::StoppedState: ui->playButton->setChecked(false); break;

    case QMediaPlayer::PlayingState: ui->playButton->setChecked(true); break;

    case QMediaPlayer::PausedState: ui->playButton->setChecked(false); break;
    }
}

void PMPlayerView::muteChanged(bool isMuted)
{
    if(isMuted) {
        ui->volumeButton->setChecked(true);
    } else {
        ui->volumeButton->setChecked(false);
    }
}

void PMPlayerView::durationChanged(qint64 duration)
{
    ui->timelineSlider->setMaximum(duration);
    m_model->durationChanged(duration);
    ui->maxTimeLabel->setText(m_model->maxMediaTime().toString("hh:mm:ss"));
}

void PMPlayerView::positionChanged(qint64 position)
{
    ui->timelineSlider->setValue(position);
    m_model->positionChanged(position);
    ui->currentTimeLabel->setText(m_model->currentMediaTime().toString("hh:mm:ss"));
}

void PMPlayerView::setToPosition(int position)
{
    m_model->setToPosition(position);
}

void PMPlayerView::removeMedia()
{
    QModelIndex index(m_playlistView->selectionModel()->selectedRows().value(0));
    m_model->removeMedia(index);
}

void PMPlayerView::previousMedia()
{
    QModelIndex newIndex = m_currentIndex.sibling(m_currentIndex.row()-1,m_currentIndex.column());
    m_model->loadMedia(newIndex);
}

void PMPlayerView::playPauseMedia()
{
    m_model->playPauseMedia();
}

void PMPlayerView::nextMedia()
{
    QModelIndex newIndex = m_currentIndex.sibling(m_currentIndex.row()+1,m_currentIndex.column());
    m_model->loadMedia(newIndex);
}

void PMPlayerView::openMedia()
{
    QUrl fileUrl = QUrl::fromLocalFile(QFileDialog::getOpenFileName(this, tr("Otworz plik")));
    if(fileUrl.isValid())
        m_model->openMedia(fileUrl);
}

void PMPlayerView::stopMedia()
{
    m_model->stopMedia();
}

void PMPlayerView::muteMedia()
{
    m_model->muteMedia();
}

void PMPlayerView::changeVolume(int)
{
    float volume = ui->volumeSlider->value() / 100.0;
    m_model->changeVolume(volume);
}

void PMPlayerView::changeSpeed()
{
    double speed = ui->speedBox->currentText().remove("x").toDouble();
    m_model->changeSpeed(speed);
}

void PMPlayerView::openFullscreen()
{

}

void PMPlayerView::colorOptions()
{

}

void PMPlayerView::currentSelectionChanged(const QModelIndex &current)
{
    m_currentIndex = current;
}

void PMPlayerView::currentElementChanged()
{
    PlaylistMediaElement element = m_model->currentElement();
    for(int i = 0; i < m_playlistView->model()->rowCount(); ++i)
    {
        QModelIndex index = m_playlistView->model()->index(i, 0);
        QString mediaName = m_playlistView->model()->data(index).toString();
        if(element.value(QMediaMetaData::Title).toString() == mediaName) {
            m_playlistView->selectRow(i);
            break;
        }
    }
}

void PMPlayerView::actionTriggered(QAction *action)
{
    if(action == addMediaAction)
    {
        openMedia();
    } else if (action == removeMediaAction) {
        removeMedia();
    }
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

    connect(m_model->player(), &QMediaPlayer::playbackStateChanged, this, &PMPlayerView::playbackStateChanged);
    connect(m_model->audioOutput(), &QAudioOutput::mutedChanged, this, &PMPlayerView::muteChanged);

    connect(m_playlistView, &PlaylistView::doubleClicked, m_model, &PMPlayerModel::loadMedia);
    connect(m_playlistView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &PMPlayerView::currentSelectionChanged);
    connect(m_model, &PMPlayerModel::currentElementChanged, this, &PMPlayerView::currentElementChanged);

    connect(m_playlistMenu, &QMenu::triggered, this, &PMPlayerView::actionTriggered);
}

void PMPlayerView::contextMenuEvent(QContextMenuEvent *event)
{
    m_playlistMenu->move(event->globalPos());
    m_playlistMenu->show();
}
