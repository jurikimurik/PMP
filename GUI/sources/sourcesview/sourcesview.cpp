#include "../../headers/sourcesview/sourcesview.h"
#include "ui_sourcesview.h"

SourcesView::SourcesView(QWidget *parent, SourcesModel *model) :
    QWidget(parent),
    ui(new Ui::SourcesView)
{
    ui->setupUi(this);

    m_sourcesView = ui->sourcesView;

    if(model)
        m_sourcesView->setModel(model);
}

SourcesView::~SourcesView()
{
    delete ui;
}

const QTreeView *SourcesView::sourcesView() const
{
    return m_sourcesView;
}
