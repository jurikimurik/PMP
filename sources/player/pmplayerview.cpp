#include "headers/player/pmplayerview.h"
#include "ui_pmplayerview.h"

PMPlayerView::PMPlayerView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PMPlayerView)
{
    ui->setupUi(this);
}

PMPlayerView::~PMPlayerView()
{
    delete ui;
}

