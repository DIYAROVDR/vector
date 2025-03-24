#include "tableview.h"

TableView::TableView(QWidget* parent) : QTableView(parent) {
    // Устанавливаем режим изменения размера секций заголовка
    horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setSectionsMovable(true);

    verticalHeader()->setVisible(false);
    //verticalHeader()->sectionSize(0);
    // Отключаем возможность выделения элементов
    setSelectionMode(QAbstractItemView::NoSelection);
}
