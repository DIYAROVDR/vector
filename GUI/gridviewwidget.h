#ifndef GRIDVIEWWIDGET_H
#define GRIDVIEWWIDGET_H

#include <QObject>
#include <QWidget>
#include <QHBoxLayout>
#include <QSplitter>

#include "selectcubewidget.h"
#include "viewsettingswidget.h"
#include "multiviewidget.h"

class GridViewWidget : public QWidget {
    Q_OBJECT
public:
    explicit GridViewWidget(QWidget* parent = nullptr);
public slots:
    void loadGrid();
    void updateGrid(size_t step);
signals:

private:
    QSplitter* spl_1;
    QSplitter* spl_2;
    MultiViewWidget* multiviewwidget;
    SelectCubeWidget* selectcubewidget;
    ViewSettingsWidget* viewsettingswidget;
};

#endif // GRIDVIEWWIDGET_H
