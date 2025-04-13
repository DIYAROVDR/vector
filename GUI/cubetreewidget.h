#ifndef CUBETREEWIDGET_H
#define CUBETREEWIDGET_H

#include <QWidget>
#include <QSortFilterProxyModel>
#include <QTreeView>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QRegExpValidator>
#include <QDebug>

#include "../GUI/ttdelegate.h"
#include "../GUI/cubestreemodel.h"

class CubeTreeWidget : public QWidget {
    Q_OBJECT
public:
    explicit CubeTreeWidget(QWidget* parent = nullptr);
    int cubeCount() const;
    QString currentCube();
    void setModel(CubesTreeModel* model);
public slots:

signals:
    void cubeItemClicked(const QString& cubeName);
    void cubeAdded(const QString& cubeName);
    void cubeDeleted(const QString& cubeName);
    void cubeNameChanged(const QString& oldName, const QString& newName);
private slots:
    void addCube();
    void delCube();
    void findCube(const QString& cubeName);
    void itemChanged(const QModelIndex& index);
    void onItemClicked(const QModelIndex& index);
private:
    int cubecount = 0;
    QTreeView* treeView = nullptr;
    QSortFilterProxyModel* proxyModel = nullptr;
    CubesTreeModel* model = nullptr;
    QLineEdit* lineEdit = nullptr;
    QLineEdit* searchBox = nullptr;
    QMap<QModelIndex, QString> oldNames;
    QFont mainFont;
    QFont boldFont;

    QWidget* createHeaderWidget();
    QString createCubeName();
};

#endif // CUBETREEWIDGET_H
