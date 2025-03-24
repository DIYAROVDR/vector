#include "cubetreewidget.h"

CubeTreeWidget::CubeTreeWidget(QWidget* parent):
    QWidget(parent),
    treeView(new QTreeView(this)),
    proxyModel(new QSortFilterProxyModel(this)),
    cubecount(0) {

    mainFont.setPointSize(10);
    boldFont.setBold(true);

    treeView->setHeaderHidden(true);
    treeView->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    treeView->setSelectionMode(QAbstractItemView::NoSelection);
    treeView->setItemDelegate(new ttDelegate(treeView));

    QWidget* headerWidget = createHeaderWidget();

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(headerWidget);
    layout->addWidget(treeView);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(1);
    setLayout(layout);
}


int CubeTreeWidget::cubeCount() const {
    return cubecount;
}


QString CubeTreeWidget::currentCube() {
    QModelIndex currentIndex = treeView->currentIndex();
    if (currentIndex.isValid()) {
        return model->data(proxyModel->mapToSource(currentIndex), Qt::DisplayRole).toString();
    }
    return QString();
}


void CubeTreeWidget::setModel(CubesTreeModel* model) {
    this->model = model;
    proxyModel->setSourceModel(model);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    treeView->setModel(proxyModel);
    connect(model, &CubesTreeModel::dataChanged, this, &CubeTreeWidget::itemChanged);
    connect(treeView, &QTreeView::clicked, this, &CubeTreeWidget::onItemClicked);
    treeView->expandAll();
}


void CubeTreeWidget::addCube() {
    QString name = createCubeName();
    QModelIndex parentIndex = model->index(2, 0); // Индекс ветки "Пользовательские кубы"

    int row = model->rowCount(parentIndex);
    model->insertRow(row, parentIndex);
    QModelIndex newIndex = model->index(row, 0, parentIndex);
    model->setData(newIndex, name, Qt::EditRole);
    emit cubeAdded(name);
    oldNames[newIndex] = name;
    ++cubecount;
}


void CubeTreeWidget::delCube() {
    QModelIndex parentIndex = model->index(2, 0); // Индекс ветки "Пользовательские кубы"
    int row = model->rowCount(parentIndex) - 1;

    if (row >= 0) {
        QModelIndex indexToDelete = model->index(row, 0, parentIndex);
        QString cubeName = model->data(indexToDelete, Qt::DisplayRole).toString();
        model->removeRow(row, parentIndex);
        oldNames.remove(indexToDelete);
        --cubecount;
        emit cubeDeleted(cubeName);
    }
}


void CubeTreeWidget::findCube(const QString& cubeName) {
    if (cubeName.isEmpty()) {
        proxyModel->setFilterRegExp("");
    } else {
        proxyModel->setFilterRegExp(QRegExp(cubeName, Qt::CaseInsensitive, QRegExp::RegExp));
    }
    treeView->expandAll();
}


void CubeTreeWidget::itemChanged(const QModelIndex& index) {
    if (index.isValid() && model->parent(index) == model->index(2, 0)) { // Проверяем, что это элемент из "Пользовательские кубы"
        QString oldName = oldNames.value(index, QString());
        QString newName = model->data(index, Qt::DisplayRole).toString();

        if (newName == oldName) {
            return;
        }

        // Проверяем уникальность имени
        QModelIndex parentIndex = model->index(2, 0);
        for (int row = 0; row < model->rowCount(parentIndex); ++row) {
            QModelIndex siblingIndex = model->index(row, 0, parentIndex);
            if (siblingIndex != index && model->data(siblingIndex, Qt::DisplayRole).toString() == newName) {
                model->setData(index, oldName, Qt::EditRole); // Возвращаем старое имя, если новое не уникально
                return;
            }
        }

        oldNames[index] = newName;
        emit cubeNameChanged(oldName, newName);
    }
}

void CubeTreeWidget::onItemClicked(const QModelIndex& index) {
    QModelIndex sourceIndex = proxyModel->mapToSource(index);

    if (sourceIndex.isValid()) {
        QString name = model->data(sourceIndex, Qt::UserRole + 1).toString();
        if (!name.isEmpty()) emit cubeItemClicked(name);
    }

    model->setData(sourceIndex, true , Qt::ForegroundRole);
}


QWidget* CubeTreeWidget::createHeaderWidget() {
    QWidget* headerWidget = new QWidget(this);
    QHBoxLayout* layout = new QHBoxLayout(headerWidget);

    QPushButton* btn_add = new QPushButton(QIcon(":/images/add.png"), "", headerWidget);
    QPushButton* btn_del = new QPushButton(QIcon(":/images/delete.png"), "", headerWidget);

    connect(btn_add, &QPushButton::clicked, this, &CubeTreeWidget::addCube);
    connect(btn_del, &QPushButton::clicked, this, &CubeTreeWidget::delCube);

    searchBox = new QLineEdit(headerWidget);
    searchBox->setPlaceholderText("Поиск кубов");

    connect(searchBox, &QLineEdit::textChanged, this, &CubeTreeWidget::findCube);

    layout->addWidget(searchBox);
    layout->addWidget(btn_add);
    layout->addWidget(btn_del);
    layout->setContentsMargins(0, 0, 5, 5);

    return headerWidget;
}


QString CubeTreeWidget::createCubeName() {
    QString baseName = "Куб";
    int suffix = 1;

    QSet<QString> existingNames;
    QModelIndex parentIndex = model->index(2, 0); // Индекс ветки "Пользовательские кубы"
    for (int row = 0; row < model->rowCount(parentIndex); ++row) {
        QModelIndex index = model->index(row, 0, parentIndex);
        existingNames.insert(model->data(index, Qt::DisplayRole).toString());
    }

    QString uniqueName;
    do {
        uniqueName = QString("%1 %2").arg(baseName).arg(suffix);
        ++suffix;
    } while (existingNames.contains(uniqueName));

    return uniqueName;
}
