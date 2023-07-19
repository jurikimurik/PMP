#include "../../headers/video/pmpvideowidget.h"
#include "ui_pmpvideowidget.h"

#include <QMouseEvent>

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
    //qDebug() << event->pos();
    event->accept();
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
