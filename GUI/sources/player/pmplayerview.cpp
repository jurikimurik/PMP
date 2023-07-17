#include "../../headers/player/pmplayerview.h"
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
    m_playlistMenu->setTitle(tr("Playlista"));
    addMediaAction = new QAction(tr("Dodaj media"), m_playlistMenu);
    removeMediaAction = new QAction(tr("Usun media"), m_playlistMenu);
    clearMediaAction = new QAction(tr("Wyczyszcz cala liste"), m_playlistMenu);
    m_playlistMenu->addActions({addMediaAction, removeMediaAction, clearMediaAction});
    m_playlistMenu->addSeparator();

    savePlaylistAction = new QAction(tr("Zapisz playlistę"), m_playlistMenu);
    loadPlaylistAction = new QAction(tr("Zaladuj playlistę"), m_playlistMenu);
    m_playlistMenu->addActions({savePlaylistAction, loadPlaylistAction});

    m_errorBox = new QErrorMessage(this);

    ui->menubar->addMenu(m_playlistMenu);

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
    case QMediaPlayer::StoppedState: ui->playButton->setChecked(false);
        ui->statusbar->showMessage(tr("Media zatrzymano."));
        break;

    case QMediaPlayer::PlayingState: ui->playButton->setChecked(true);
        ui->statusbar->showMessage(tr("Odtwarzanie media..."));
        break;

    case QMediaPlayer::PausedState: ui->playButton->setChecked(false);
        ui->statusbar->showMessage(tr("Media wstrzymano."));
        break;
    }
}

void PMPlayerView::playerStatusUpdated(QMediaPlayer::MediaStatus status)
{
    switch(status)
    {
    case QMediaPlayer::NoMedia:
        ui->statusbar->showMessage(tr("Brak zaladowanego media."));
        break;

    case QMediaPlayer::LoadingMedia:
        ui->statusbar->showMessage(tr("Media się ładuje..."));
        break;

    case QMediaPlayer::LoadedMedia:
        ui->statusbar->showMessage(tr("Media zostało pomyśle załadaowane..."));
        break;

    case QMediaPlayer::StalledMedia:
        ui->statusbar->showMessage(tr("Problem z załadowaniem, operacja potrwa dłużej..."));
        break;

    case QMediaPlayer::BufferingMedia:
        ui->statusbar->showMessage(tr("Media się ładuję do bufforu, ale może być włączone..."));
        break;

    case QMediaPlayer::BufferedMedia:
        ui->statusbar->showMessage(tr("Media zostało całkiem załadowane do buffora."));
        break;

    case QMediaPlayer::EndOfMedia:
        ui->statusbar->showMessage(tr("Koniec media."));
        break;

    case QMediaPlayer::InvalidMedia:
        ui->statusbar->showMessage(tr("UWAGA! NIEPOPRAWNY PLIK MEDIA!"));
        break;
    };
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

void PMPlayerView::savePlaylist()
{
    QString pathname = QFileDialog::getSaveFileName(this, tr("Zapisz playliste..."), QString(), tr("PLaylista PMP (*.pmplst)"));

    if(pathname.isEmpty())
        return;

    if(!m_model->savePlaylistToFile(pathname)) {
        m_errorBox->showMessage(tr("Nie udało się zapisać pliku!"));
    }
}

void PMPlayerView::loadPlaylist()
{
    QString pathname = QFileDialog::getOpenFileName(this, tr("Zapisz playliste..."), QString(), tr("PLaylista PMP (*.pmplst)"));

    if(pathname.isEmpty())
        return;

    if(!m_model->loadPlaylistFromFile(pathname)) {
        m_errorBox->showMessage(tr("Nie udało się otworzyć pliku!"));
    }

    m_model->loadMedia(m_currentIndex);
}

void PMPlayerView::clearAllMedia()
{
    m_model->clearMedia();
}

void PMPlayerView::removeMedia()
{
    m_model->removeMedia(m_playlistView->selectionModel()->selectedRows());
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
    QStringList listOfPathes = QFileDialog::getOpenFileNames(this, tr("Otworz pliki media"));
    for(const QString& path : listOfPathes)
    {
        QUrl fileUrl = QUrl::fromLocalFile(path);
        if(fileUrl.isValid())
            m_model->openMedia(fileUrl);
    }

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
    } else if (action == clearMediaAction) {
        clearAllMedia();
    } else if(action == savePlaylistAction) {
        savePlaylist();
    } else if(action == loadPlaylistAction) {
        loadPlaylist();
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
    connect(m_model->player(), &QMediaPlayer::mediaStatusChanged, this, &PMPlayerView::playerStatusUpdated);
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
