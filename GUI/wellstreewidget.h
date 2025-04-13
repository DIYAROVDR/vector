#ifndef WELLSTREEWIDGET_H
#define WELLSTREEWIDGET_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTreeView>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QStandardItem>
#include <QRegExpValidator>

#include "../GUI/ttdelegate.h"
#include "../GUI/wellsproxymodel.h"

class WellsTreeWidget : public QWidget {
    Q_OBJECT
public:
    explicit WellsTreeWidget(QWidget* parent = nullptr);
    int wellCount() const;
    QString currentWell();
signals:
    void wellItemClicked(const QString& wellName);
    void wellAdded(const QString& wellName);
    void wellDeleted(const QString& wellName);
    void wellNameChanged(const QString& oldName, const QString& newName);
private slots:
    void addWell();
    void delWell();
    void findWell(const QString& wellName);
    void itemChanged(QStandardItem* item);
private:
    int wellcount = 0;
    QTreeView* treeView = nullptr;
    WellsProxyModel* proxyModel = nullptr;
    QStandardItemModel* model = nullptr;
    QStandardItem* wellsItem = nullptr;
    QStandardItem* previousWellItem = nullptr;
    QStandardItem* currentWellItem = nullptr;
    QLineEdit* lineEdit = nullptr;
    QLineEdit* searchBox = nullptr;
    QMap<QStandardItem*, QString> oldNames;
    QFont mainFont;
    QFont boldFont;

    QWidget* createHeaderWidget();
    QString createWellName();
};

#endif // WELLSTREEWIDGET_H
