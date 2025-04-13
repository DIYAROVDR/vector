#ifndef HOMEWIDGET_H
#define HOMEWIDGET_H

#include <QWidget>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QSpacerItem>

#include "../GUI/aspectratiopixmaplabel.h"

class HomeWidget : public QWidget {
    Q_OBJECT
public:
    explicit HomeWidget(QWidget *parent = nullptr);
    ~HomeWidget();

signals:
    void createProject(const QString& dir);

private slots:
    void on_btn_create_new_project_clicked();

private:
    QString projectName;
    QString projectDir;
    QPushButton* createButton(const QString& dir, const QString& projectName);

    QPushButton* btn_create_new_project;
    QPushButton* btn_open_project;
    AspectRatioPixmapLabel* label;
    QLabel* lbl_project;
    QScrollArea* area;
    QWidget* scrollAreaWidgetContents;
};

#endif // HOMEWIDGET_H
