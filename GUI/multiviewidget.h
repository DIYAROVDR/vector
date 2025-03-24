#ifndef MULTIVIEWIDGET_H
#define MULTIVIEWIDGET_H

#include <QTabWidget>
#include <QTabBar>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

#include "../GUI/addviewwidget.h"
#include "../Core/h5filemanager.h"

class MultiViewWidget : public QTabWidget {
    Q_OBJECT
public:
    explicit MultiViewWidget(QWidget* parent = nullptr);
public slots:
    void loadGrid();
    void updateGrid(size_t step);
    void showAxis(bool flag);
    void showOrientationAxis(bool flag);
    void showLegend(bool flag);

private:
    AddViewWidget* addwgt;
    QFont mainfont;
    void addCloseButton(int index);
};

#endif // MULTIVIEWIDGET_H
