#ifndef SOURCESVIEW_H
#define SOURCESVIEW_H

#include <QTreeView>
#include <QWidget>

#include "Backend/headers/sourcesmodel/sourcesmodel.h"

namespace Ui {
class SourcesView;
}

class SourcesView : public QWidget
{
    Q_OBJECT

public:
    explicit SourcesView(QWidget *parent = nullptr, SourcesModel *model = nullptr);
    ~SourcesView();
    const QTreeView* sourcesView() const;

private:
    Ui::SourcesView *ui;
    QTreeView *m_sourcesView;
};

#endif // SOURCESVIEW_H
