#ifndef ROCKDATAFORM_H
#define ROCKDATAFORM_H

#include <QWidget>
#include <QHeaderView>
#include <QFont>
#include <QDebug>

#include "creatorwidgets.h"
#include "units.h"

#include "../Core/rockdata.h"


class RockDataWidget : public QWidget {
    Q_OBJECT

public:
    explicit RockDataWidget(QWidget *parent = nullptr);
    void openProject();
    ~RockDataWidget();
signals:
    void applyButtonClicked(RockData* rock);
public slots:
    void setRegionCount(int count);
private slots:
    void on_btn_apply_clicked();
private:
    int regnum;
    QFont mainFont;
    std::vector<RockData> rock;
    std::vector<UnitType> units;
    QStringList headers;
};

#endif // ROCKDATAFORM_H
