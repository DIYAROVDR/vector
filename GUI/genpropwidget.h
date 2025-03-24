#ifndef GENPROPWIDGET_H
#define GENPROPWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QButtonGroup>

#include "creatorwidgets.h"

#include "../Core/physicalquantity.h"
#include "generalmodel.h"
#include "multidelegate.h"

#include "treeview.h"

class GenPropWidget : public QWidget {
    Q_OBJECT
public:
    explicit GenPropWidget(QWidget* parent = nullptr);
    ~GenPropWidget();
    void openProject(PhysicalQuantity* physicalQuantity);
signals:

private slots:

private:
    TreeView* treeView;
    PhysicalQuantity* physicalQuantity;

    QFont mainFont;
    QFont boldFont;
};




#endif // GENPROPWIDGET_H
