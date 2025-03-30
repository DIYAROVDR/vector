#ifndef FILLCUBEWIDGET_H
#define FILLCUBEWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QPlainTextEdit>
#include <QSplitter>
#include <QMessageBox>
#include <QTableView>
#include <QTabWidget>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "creatorwidgets.h"
#include "codeeditor.h"
#include "customoutputwindow.h"
#include "pythonmodule.h"
#include "cubetreewidget.h"
#include "layerfillcubemodel.h"
#include "multidelegate.h"
#include "tableview.h"

#include "../Core/h5filemanager.h"
#include "../Core/physicalquantity.h"

class FillCubeWidget : public QWidget {
    Q_OBJECT

public:
    explicit FillCubeWidget(QWidget* parent, PhysicalQuantity* physicalquantity);
    ~FillCubeWidget();
    void setProjectPath(const QString& path);
    void openProject();

public slots:
    void dimensChanged();

private slots:
    void on_btn_run_script_clicked();
    void on_btn_clear_code_clicked();
    void on_btn_add_script_clicked();
    void on_btn_delete_script_clicked();
    void on_btn_apply_clicked();

private:
    QDoubleValidator* doubleValidator = nullptr;
    H5FileManager& h5filemanager;
    CubeTreeWidget* cubetreewidget = nullptr;
    QSplitter* splmain = nullptr;
    QSplitter* spl = nullptr;
    QPlainTextEdit* plainTextEdit_result = nullptr;
    CodeEditor* codeEditor = nullptr;
    PythonModule* pythonModule = nullptr;
    PhysicalQuantity* physicalquantity;

    LayerFillCubeModel* layerfillcubemodel;
    CubesTreeModel* cubestreemodel;

    QMap<QString, QString> scripts;

    QTabWidget* tabWidget;
    QTableView* tableView;
    QComboBox* cmb_scripts_lists;
    QComboBox* cmb_type_edit_tab;
    QPushButton* btn_apply_table_data;
    QPushButton* btn_clear_tabe;
    QPushButton* btn_add_script;
    QPushButton* btn_delete_script;
    QPushButton* btn_load_script;
    QPushButton* btn_clear_code;
    QPushButton* btn_run_script;
};

#endif // FILLCUBEWIDGET_H
