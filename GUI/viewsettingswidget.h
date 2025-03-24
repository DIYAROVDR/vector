#ifndef VIEWSETTINGSWIDGET_H
#define VIEWSETTINGSWIDGET_H

#include <QWidget>


class ViewSettingsWidget : public QWidget {
    Q_OBJECT

public:
    explicit ViewSettingsWidget(QWidget *parent = nullptr);
    ~ViewSettingsWidget();

signals:
    void showAxis(bool flag);
    void showOrientationAxis(bool flag);
    void showLegend(bool flag);
    void show2D(bool flag);
    void show3D(bool flag);

private slots:

private:

};

#endif // VIEWSETTINGSWIDGET_H
