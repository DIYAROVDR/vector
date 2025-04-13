#ifndef ADDVIEWWIDGET_H
#define ADDVIEWWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QGridLayout>
#include "../GUI/spatialviewwidget.h"

class AddViewWidget : public QWidget {
    Q_OBJECT
public:
    explicit AddViewWidget(QWidget *parent = nullptr);
    ~AddViewWidget();
    SpatialViewWidget* createSpatialViewWidget();
signals:
    void createSpatialViewWidgetBtnClicked(SpatialViewWidget* wgt);
private slots:
    void on_btn_create_spatial_view_part_clicked();
private:
    QPushButton* btn_create_spatial_view_part;
    QPushButton* btn_create_spatial_view_4part;
    QPushButton* btn_create_histogram_view;
    QPushButton* btn_create_plot_view;
    QPushButton* btn_create_table_view;
    QPushButton* btn_create_well_profile_view;
    QPushButton* btn_crate_user_view;
    QLabel* label_main;
    QSpacerItem* spacer_top;
    QSpacerItem* spacer_bottom;
    QSpacerItem* spacer_left;
    QSpacerItem* spacer_right;
    QVBoxLayout* mainLayout;
    QGridLayout* buttonLayout;
    QFont maintFont;
    QFont boldFont;
};

#endif // ADDVIEWWIDGET_H
