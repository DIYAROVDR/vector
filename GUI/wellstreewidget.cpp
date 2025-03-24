#include "wellstreewidget.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QRegExpValidator>

WellsTreeWidget::WellsTreeWidget(QWidget* parent):
                QWidget(parent),
                treeView(new QTreeView(this)),
                model(new QStandardItemModel(this)),
                proxyModel(new WellsProxyModel(this)),
                wellcount(0) {

    mainFont.setPointSize(10);
    boldFont.setBold(true);

    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    treeView->setModel(proxyModel);
    treeView->setHeaderHidden(true);
    treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    treeView->setSelectionMode(QAbstractItemView::NoSelection);

    wellsItem = new QStandardItem("Скважины");
    wellsItem->setFont(boldFont);
    wellsItem->setBackground(QColor(173, 216, 230));
    model->appendRow(wellsItem);

    connect(model, &QStandardItemModel::itemChanged, this, &WellsTreeWidget::itemChanged);

    QWidget* headerWidget = createHeaderWidget();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(headerWidget);
    layout->addWidget(treeView);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(1);
    setLayout(layout);

    connect(treeView, &QTreeView::clicked, this, [=](const QModelIndex& index) {
        // Получаем элемент, на который был произведен клик
        QStandardItem* item = model->itemFromIndex(proxyModel->mapToSource(index));

        if (item && item->parent() == wellsItem) {
            currentWellItem = item; // Обновляем текущую скважину

            // Сбрасываем фон у предыдущего элемента, если он есть
            if (previousWellItem && previousWellItem != item) {
                previousWellItem->setBackground(Qt::NoBrush); // Убираем фон
            }

            // Устанавливаем нейтральный фон для текущего элемента
            item->setBackground(QBrush(QColor(200, 200, 200))); // Светло-серый цвет

            // Сохраняем текущий элемент как предыдущий
            previousWellItem = item;

            emit wellItemClicked(item->text());
        }
    });

    treeView->setItemDelegate(new ttDelegate(treeView));
    treeView->expandAll();
}


int WellsTreeWidget::wellCount() const {
    return wellcount;
}

QString WellsTreeWidget::currentWell() {
    return currentWellItem ? currentWellItem->text() : QString();
}

void WellsTreeWidget::addWell() {
    QString name = createWellName();
    QStandardItem* newWellItem = new QStandardItem(name);
    wellsItem->appendRow(newWellItem);
    emit wellAdded(name);
    oldNames[newWellItem] = name ;
    ++wellcount;
}

void WellsTreeWidget::delWell() {
    QStandardItem* itemToDelete = nullptr;

    // Если текущий элемент выбран, удаляем его
    if (currentWellItem) {
        itemToDelete = currentWellItem;
    }
    // Если текущий элемент не выбран, удаляем последний элемент
    else if (wellsItem->rowCount() > 0) {
        itemToDelete = wellsItem->child(wellsItem->rowCount() - 1);
    }

    // Удаляем найденный элемент
    if (itemToDelete) {
        QString wellName = itemToDelete->text(); // Имя удаляемой скважины

        // Удаляем элемент из wellsItem
        wellsItem->removeRow(itemToDelete->row());

        // Обновляем счетчик скважин
        --wellcount;

        // Если удаляем текущую скважину, сбрасываем текущий элемент
        if (itemToDelete == currentWellItem) {
            currentWellItem = nullptr;
        }

        // Сбрасываем предыдущее выделение, если удаленный элемент был выделен
        if (itemToDelete == previousWellItem) {
            previousWellItem = nullptr;
        }

        // Удаляем имя из oldNames
        oldNames.remove(itemToDelete);

        // Генерируем сигнал об удалении скважины
        emit wellDeleted(wellName);
    }
}




void WellsTreeWidget::findWell(const QString& wellName) {
    if (wellName.isEmpty()) {
        proxyModel->setFilterRegExp("");
    } else {
        proxyModel->setFilterRegExp(QRegExp(wellName, Qt::CaseInsensitive, QRegExp::RegExp));
    }
    treeView->expandAll();
}


void WellsTreeWidget::itemChanged(QStandardItem* item) {
    if (item && item->parent() == wellsItem) {
        QString oldName = oldNames.value(item, QString());

        QString newName = item->text();

        if (newName == oldName) {
            return;
        }

        QSet<QString> existingNames;
        for (int row = 0; row < wellsItem->rowCount(); ++row) {
            QStandardItem* sibling = wellsItem->child(row);
            if (sibling && sibling != item) {
                existingNames.insert(sibling->text());
            }
        }

        if (existingNames.contains(newName)) {
            item->setText(oldName);
            return;
        }

        oldNames[item] = newName;

        emit wellNameChanged(oldName, newName);
    }
}




QWidget* WellsTreeWidget::createHeaderWidget() {
    QWidget* headerWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(headerWidget);

    QPushButton* btn_add = new QPushButton(QIcon(":/images/add.png"), "", headerWidget);
    QPushButton* btn_del = new QPushButton(QIcon(":/images/delete.png"), "", headerWidget);

    connect(btn_add, &QPushButton::clicked, this, &WellsTreeWidget::addWell);
    connect(btn_del, &QPushButton::clicked, this, &WellsTreeWidget::delWell);


    searchBox = new QLineEdit(headerWidget);
    searchBox->setPlaceholderText("Поиск скважин");

    connect(searchBox, &QLineEdit::textChanged, this, &WellsTreeWidget::findWell);

    layout->addWidget(searchBox);
    layout->addWidget(btn_add);
    layout->addWidget(btn_del);
    layout->setContentsMargins(0, 0, 5, 5);

    return headerWidget;
}

QString WellsTreeWidget::createWellName() {
    QString baseName = "Скважина";
    int suffix = 1;

    // Сбор всех существующих имен (включая вложенные элементы)
    QSet<QString> existingNames;
    std::function<void(QStandardItem*)> collectNames = [&](QStandardItem* item) {
        if (!item) return;
        existingNames.insert(item->text());
        for (int i = 0; i < item->rowCount(); ++i) {
            collectNames(item->child(i));
        }
    };

    for (int row = 0; row < model->rowCount(); ++row) {
        collectNames(model->item(row));
    }

    // Генерация уникального имени
    QString uniqueName;
    do {
        uniqueName = QString("%1 %2").arg(baseName).arg(suffix);
        ++suffix;
    } while (existingNames.contains(uniqueName));

    return uniqueName;
}

