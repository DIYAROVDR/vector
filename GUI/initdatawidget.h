#ifndef INITDATAWIDGET_H
#define INITDATAWIDGET_H

#include <QWidget>

class InitDataWidget : public QWidget {
    Q_OBJECT

public:
    explicit InitDataWidget(QWidget* parent = nullptr);
    void setRegionCount(int count);
    void openProject();
    ~InitDataWidget();

private:
    int regnum = 1;
    QStringList headers {
        "Регионы",
        "Глубина",
        "Давление на этой глубине",
        "Глубина ВНК",
        "Капилярное давление на глубине ВНК",
        "Глубина ГНК",
        "Иниц. газир. нефть RSVD/PBVD",
        "Иниц. жирный газ RVVD/PDVD"
    };
};

#endif // INITDATAWIDGET_H
