#ifndef PMPLAYERVIEW_H
#define PMPLAYERVIEW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class PMPlayerView; }
QT_END_NAMESPACE

class PMPlayerView : public QMainWindow
{
    Q_OBJECT

public:
    PMPlayerView(QWidget *parent = nullptr);
    ~PMPlayerView();

private:
    Ui::PMPlayerView *ui;
};
#endif // PMPLAYERVIEW_H
