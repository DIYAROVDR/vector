#include "treeview.h"

TreeView::TreeView(QWidget* parent) : QTreeView(parent) {
    // Настройка режима изменения размеров секций заголовка
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    
    // Настройка режима выделения
    setSelectionMode(QAbstractItemView::NoSelection);
    
    // Настройка триггеров редактирования (одиночный клик)
    setEditTriggers(QAbstractItemView::AnyKeyPressed | CurrentChanged);
    // Дополнительные настройки, если нужно
}