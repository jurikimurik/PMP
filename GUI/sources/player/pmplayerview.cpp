#include "../../headers/player/pmplayerview.h"
#include "ui_pmplayerview.h"

#include <QGraphicsVideoItem>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QFileDialog>
#include <QMessageBox>

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
    m_playlistView->setModel(m_model->currentPlaylist());

    ui->playlistNameEdit->setText(m_model->currentPlaylist()->playlistName());

    m_splitter = new QSplitter(Qt::Horizontal);
    ui->graphicsView->setParent(m_splitter);
    m_splitter->addWidget(ui->graphicsView);
    m_splitter->addWidget(m_playlistView);

    ui->mediaWidget->layout()->addWidget(m_splitter);

    m_playlistMenu = new QMenu(m_playlistView);
    m_playlistMenu->setTitle(tr("Edytuj"));

    copyMediaAction = new QAction(tr("Kopiuj"), m_playlistMenu);
    cutMediaAction = new QAction(tr("Wytnij"), m_playlistMenu);
    insertHereAction = new QAction(tr("Wklej tutaj"), m_playlistMenu);
    m_playlistMenu->addActions({copyMediaAction, cutMediaAction, insertHereAction});
    m_playlistMenu->addSeparator();

    playAsNextAction = new QAction(tr("Odtwórz jako następny"), m_playlistMenu);
    addToQueueAction = new QAction(tr("Dodaj do kolejki"), m_playlistMenu);
    removeFromQueue = new QAction(tr("Usun z kolejki"), m_playlistMenu);
    m_playlistMenu->addActions({playAsNextAction, addToQueueAction, removeFromQueue});
    m_playlistMenu->addSeparator();

    addMediaAction = new QAction(tr("Dodaj media"), m_playlistMenu);
    removeMediaAction = new QAction(tr("Usun media"), m_playlistMenu);
    clearMediaAction = new QAction(tr("Wyczyszcz cala liste"), m_playlistMenu);
    m_playlistMenu->addActions({addMediaAction, removeMediaAction, clearMediaAction});
    m_playlistMenu->addSeparator();

    m_mainMenu = new QMenu(tr("Playlist"), this);
    savePlaylistAction = new QAction(tr("Zapisz playlistę"), m_mainMenu);
    loadPlaylistAction = new QAction(tr("Zaladuj playlistę"), m_mainMenu);
    m_mainMenu->addActions({savePlaylistAction, loadPlaylistAction});
    ui->menubar->addMenu(m_mainMenu);
    ui->menubar->addMenu(m_playlistMenu);

    m_errorBox = new QErrorMessage(this);

    m_videoWidget = new PMPVideoWidget(this);
    m_model->setVideoOutput(m_videoWidget->videoWidget());
    QWidget *returned = m_splitter->replaceWidget(m_splitter->indexOf(ui->graphicsView), m_videoWidget);
    if(returned != nullptr)
        returned->deleteLater();
    m_videoWidget->show();

    ui->menubar->addMenu(m_playlistMenu);

    createConnections();
    durationChanged(m_model->player()->duration());

    QString searchedSpeed(QString("x %1").arg(m_model->player()->playbackRate()));
    ui->speedBox->setCurrentIndex(ui->speedBox->findText(searchedSpeed, Qt::MatchContains));

    readSettings();
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
        stopMedia();
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
    QString pathname = QFileDialog::getSaveFileName(this, tr("Zapisz playliste..."), QString(), tr("M3U Format (*.m3u);;Playlista PMP (*.pmplst)"));

    if(pathname.isEmpty())
        return;

    if(!m_model->savePlaylistToFile(pathname)) {
        m_errorBox->showMessage(tr("Nie udało się zapisać pliku!"));
    }
}

void PMPlayerView::loadPlaylist()
{
    QString pathname = QFileDialog::getOpenFileName(this, tr("Otwórz playliste..."), QString(), tr("M3U Format (*.m3u);;Playlista PMP (*.pmplst)"));

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

void PMPlayerView::removeMedia(const QModelIndexList &indexes)
{
    m_model->removeMedia(indexes);
}

void PMPlayerView::previousMedia()
{
    m_model->previousMedia();
}

void PMPlayerView::playPauseMedia()
{
    m_model->playPauseMedia();
}

void PMPlayerView::nextMedia()
{
    m_model->nextMedia();
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

void PMPlayerView::playedMediaChanged()
{
    m_currentIndex = m_model->currentIndexPlaying();
    m_playlistView->selectRow(m_currentIndex.row());
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
    } else if(action == playAsNextAction) {
        m_model->playAsNext(m_playlistView->selectionModel()->selectedRows().value(0));
    } else if(action == addToQueueAction) {
        m_model->addToQueue(m_playlistView->selectionModel()->selectedRows().value(0));
    } else if(action == removeFromQueue) {
        m_model->removeFromQueue(m_playlistView->selectionModel()->selectedRows().value(0));
    } else if(action == copyMediaAction) {
        copyToClipboard();
    } else if(action == cutMediaAction) {
        cutToClipboard();
    } else if(action == insertHereAction) {
        pasteFromClipboard();
    }
}


void PMPlayerView::createConnections()
{
    connect(m_model->player(), &QMediaPlayer::durationChanged, this, &PMPlayerView::durationChanged);
    connect(m_model->player(), &QMediaPlayer::positionChanged, this, &PMPlayerView::positionChanged);
    connect(ui->timelineSlider, &QSlider::sliderMoved, this, &PMPlayerView::setToPosition);

    connect(ui->playlistNameEdit, &QLineEdit::textEdited, m_model, &PMPlayerModel::setPlaylistName);
    connect(m_model, &PMPlayerModel::playlistNameChanged, ui->playlistNameEdit, &QLineEdit::setText);

    connect(ui->stopButton, &QPushButton::clicked, this, &PMPlayerView::stopMedia);
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

    connect(m_playlistView, &PlaylistView::doubleClicked, m_model, qOverload<const QModelIndex&>(&PMPlayerModel::loadMedia));
    connect(m_model, &PMPlayerModel::currentIndexPlayingChanged,
            this, &PMPlayerView::playedMediaChanged);

    connect(m_playlistMenu, &QMenu::triggered, this, &PMPlayerView::actionTriggered);
    connect(m_mainMenu, &QMenu::triggered, this, &PMPlayerView::actionTriggered);
}

void PMPlayerView::readSettings()
{
    QSettings settings;

    QByteArray state = settings.value("windowState", QByteArray()).toByteArray();
    QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
    QByteArray playlistData = settings.value("currentplaylist", QByteArray()).toByteArray();
    int currentRow = settings.value("currentrowelement", -1).toInt();

    if(!state.isEmpty())
        restoreState(state);
    if(!geometry.isEmpty())
        restoreGeometry(geometry);

    if(!playlistData.isEmpty()) {
        QFile tempFile("tempFile");
        QFileInfo info(tempFile);
        if(!tempFile.open(QIODevice::ReadWrite)) {
            qDebug() << "Attention: Can't create temp file!";
            return;
        }
        tempFile.write(playlistData);
        tempFile.close();
        m_model->loadPlaylistFromFile(info.absoluteFilePath());
        tempFile.remove();
    }

    if(currentRow != -1) {
        m_playlistView->selectRow(currentRow);
        m_model->loadMedia(m_playlistView->selectionModel()->selectedRows().value(0));
    }


}

void PMPlayerView::writeSettings()
{
    QSettings settings;

    //Save all playlist to file, read from it and then delete temporary file.
    QFile tempFile("tempFile.m3u");
    QFileInfo info(tempFile);
    if(!tempFile.open(QIODevice::ReadWrite)) {
        qDebug() << "Attention: Can't create temp file!";
        return;
    }
    m_model->savePlaylistToFile(info.absoluteFilePath());
    QByteArray tempFileData = tempFile.readAll();
    tempFile.close();
    tempFile.remove();

    settings.setValue("windowState", saveState());
    settings.setValue("geometry", saveGeometry());
    settings.setValue("currentplaylist", tempFileData);
    settings.setValue("currentrowelement", m_currentIndex.row());
}

bool PMPlayerView::areUserWantsToQuit()
{
    int answer = QMessageBox::question(this, tr("Pytanie"), tr("Naprawde chcesz wyjść?"),
                                       {QMessageBox::Yes | QMessageBox::No}, QMessageBox::No);
    if(answer == QMessageBox::Yes)
        return true;
    else
        return false;
}

void PMPlayerView::copyToClipboard()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString data;
    QModelIndexList list = m_playlistView->selectionModel()->selectedRows();
    for(const QModelIndex &index : list)
        data += m_model->currentPlaylist()->getSourceURL(index).toString() + "\n";
    clipboard->setText(data);
}

void PMPlayerView::cutToClipboard()
{
    copyToClipboard();
    removeMedia();
}

void PMPlayerView::pasteFromClipboard()
{

    QStringList urlsText = QGuiApplication::clipboard()->text().split("\n");
    QVector<QUrl> urls = QUrl::fromStringList(urlsText);

    QModelIndex selectedIndex = m_playlistView->selectionModel()->selectedRows().value(0);
    m_model->insertMedia(urls, selectedIndex);
}

void PMPlayerView::contextMenuEvent(QContextMenuEvent *event)
{
    m_playlistMenu->move(event->globalPos());
    m_playlistMenu->show();
}

void PMPlayerView::closeEvent(QCloseEvent *event)
{
    if(areUserWantsToQuit()) {
        writeSettings();
        event->accept();
    } else {
        event->ignore();
    }

}
