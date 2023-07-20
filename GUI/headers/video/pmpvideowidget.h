#ifndef PMPVIDEOWIDGET_H
#define PMPVIDEOWIDGET_H

#include <QMenu>
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

signals:
    void screenClicked();

public slots:
    void fullscreenOnOff();

private:
    Ui::PMPVideoWidget *ui;
    QVideoWidget *m_videoWidget;

    bool isFullscreenNow = false;
    QLayout *m_parentLayout;
    QWidget *m_widgetInstead;

    QMenu *m_playerMenu;
    QAction *m_ignoreRatio;
    QAction *m_keepRatio;
    QAction *m_keepExpandingRatio;

    // QWidget interface
    void intializeVideoMenu();

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void closeEvent(QCloseEvent *event);

    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void actionTriggered(QAction *action);
};

#endif // PMPVIDEOWIDGET_H
