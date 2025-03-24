#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>
#include <QSplitter>
#include <QThread>
#include <QVBoxLayout>
#include <QPushButton>
#include <QStackedWidget>

#include "../Core/solver.h"
#include "../Core/h5filemanager.h"

class HomeWidget;
class ProjectDataWidget;
class GridViewWidget;
class TimeSliderWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_btn_run_clicked();
    void onButtonClicked(QAbstractButton* button);

private:
    QFont mainFont;
    QButtonGroup* buttonGroup;
    QAbstractButton* currentButton;
    QSplitter* spl;
    Solver* solver;
    QThread* thread;

    QWidget* centralWidget;
    QVBoxLayout* mainLayout;
    TimeSliderWidget* timeSliderWidget;
    QWidget* mainWidgets;
    QHBoxLayout* hLayout;
    QWidget* btnWidget;
    QVBoxLayout* btnLayout;
    QPushButton* btnHome;
    QPushButton* btnModel;
    QPushButton* btnGrid;
    QPushButton* btnPlot;
    QSpacerItem* verticalSpacer;
    QStackedWidget* stackedWidget;
    QWidget* pageHome;
    QVBoxLayout* homeLayout;
    HomeWidget* homeWidget;
    QWidget* pageModel;
    QVBoxLayout* modelLayout;
    ProjectDataWidget* modelDataWidget;
    QWidget* pageGrid;
    QVBoxLayout* gridLayout;
    GridViewWidget* gridViewWidget;
    QWidget* pagePlot;
};

#endif // MAINWINDOW_H
