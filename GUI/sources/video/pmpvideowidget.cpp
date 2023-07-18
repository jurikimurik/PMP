#include "../../headers/video/pmpvideowidget.h"
#include "ui_pmpvideowidget.h"

PMPVideoWidget::PMPVideoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PMPVideoWidget)
{
    ui->setupUi(this);

    m_videoWidget = new QVideoWidget(this);
    ui->horizontalLayout->addWidget(m_videoWidget);

    //Default settings
    m_videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_videoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
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

}
