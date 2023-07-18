#ifndef PMPVIDEOWIDGET_H
#define PMPVIDEOWIDGET_H

#include <QVideoWidget>
#include <QWindow>

namespace Ui {
class PMPVideoWidget;
}

class PMPVideoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PMPVideoWidget(QWidget *parent = nullptr);
    ~PMPVideoWidget();

    QVideoWidget *videoWidget() const;

public slots:
    void fullscreenOnOff();

private:
    Ui::PMPVideoWidget *ui;
    QVideoWidget *m_videoWidget;

    bool isFullscreenNow = false;
    QLayout *m_parentLayout;
    QWidget *m_widgetInstead;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
};

#endif // PMPVIDEOWIDGET_H
