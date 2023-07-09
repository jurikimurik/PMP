#ifndef PMPLAYERVIEW_H
#define PMPLAYERVIEW_H

#include <QMainWindow>
#include "pmplayermodel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PMPlayerView; }
QT_END_NAMESPACE

class PMPlayerView : public QMainWindow
{
    Q_OBJECT

public:
    PMPlayerView(QWidget *parent = nullptr, PMPlayerModel *model = nullptr);
    ~PMPlayerView();

public slots:
    void statusChanged();
private:
    Ui::PMPlayerView *ui;
    PMPlayerModel* m_model;

};
#endif // PMPLAYERVIEW_H
