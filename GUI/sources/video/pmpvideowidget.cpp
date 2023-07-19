#include "../../headers/video/pmpvideowidget.h"
#include "ui_pmpvideowidget.h"

#include <QMouseEvent>
#include <QTimer>

PMPVideoWidget::PMPVideoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PMPVideoWidget)
{
    ui->setupUi(this);

    m_videoWidget = new QVideoWidget(this);
    ui->verticalLayout->addWidget(m_videoWidget);

    //Default settings
    m_videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_videoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
    setWindowTitle(tr("Odtwarzacz multimedia"));

    intializeVideoMenu();
}

void PMPVideoWidget::intializeVideoMenu()
{
    m_playerMenu = new QMenu(tr("Opcje odtwarzania"), this);
    m_ignoreRatio = new QAction(tr("Ignoruj proporcje"), this);
    m_keepRatio = new QAction(tr("Dostosowanie proporcji"), this);
    m_keepExpandingRatio = new QAction(tr("Dostosuj proporcje i poszerz"), this);
    m_playerMenu->addActions({m_ignoreRatio, m_keepRatio, m_keepExpandingRatio});
    connect(m_playerMenu, &QMenu::triggered, this, &PMPVideoWidget::actionTriggered);
}


PMPVideoWidget::~PMPVideoWidget()
{
    delete ui;
}

QVideoWidget *PMPVideoWidget::videoWidget() const
{
    return m_videoWidget;
}

void PMPVideoWidget::fullscreenOnOff()
{
    //We create QWidget instead of video player window.
    //When user want to return back, we simply replacing new created widget with video player.
    if(isFullscreenNow) {
        m_parentLayout->replaceWidget(m_widgetInstead, this);
        setWindowFlags({Qt::Widget});
        m_widgetInstead->deleteLater();
        this->show();
        isFullscreenNow = false;
    } else {
        m_parentLayout = parentWidget()->layout();
        m_widgetInstead = new QWidget(nullptr);
        m_parentLayout->replaceWidget(this, m_widgetInstead);
        setWindowFlags({Qt::Window, Qt::FramelessWindowHint});

        this->setParent(nullptr, {Qt::WindowFullScreen});
        this->showFullScreen();
        isFullscreenNow = true;
    }
}

void PMPVideoWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit playPause();
        event->ignore();
    } else {
        QWidget::mousePressEvent(event);
    }
}

void PMPVideoWidget::mouseReleaseEvent(QMouseEvent *event)
{

}

void PMPVideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    fullscreenOnOff();
}

void PMPVideoWidget::mouseMoveEvent(QMouseEvent *event)
{

}

void PMPVideoWidget::closeEvent(QCloseEvent *event)
{
    if(isFullscreenNow) {
        fullscreenOnOff();
        event->ignore();
    } else {
        QWidget::closeEvent(event);
    }
}

void PMPVideoWidget::contextMenuEvent(QContextMenuEvent *event)
{
    m_playerMenu->move(event->globalPos());
    m_playerMenu->show();
}

void PMPVideoWidget::actionTriggered(QAction *action)
{
    if(action == m_ignoreRatio)
    {
        m_videoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
    } else if (action == m_keepRatio) {
        m_videoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
    } else if (action == m_keepExpandingRatio) {
        m_videoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
    }
}
