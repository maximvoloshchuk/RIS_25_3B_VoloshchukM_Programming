#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QDialog>
#include <QMessageBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget* central = new QWidget;
    setCentralWidget(central);
    QHBoxLayout* mainLayout = new QHBoxLayout(central);

    // Левая панель управления
    QVBoxLayout* leftPanel = new QVBoxLayout;

    // ---- Вершины ----
    QGroupBox* vertexBox = new QGroupBox("Вершины");
    QVBoxLayout* vLayout = new QVBoxLayout;
    QPushButton* addVertexBtn = new QPushButton("Добавить вершину");
    QPushButton* removeVertexBtn = new QPushButton("Удалить вершину (выберите)");
    vLayout->addWidget(addVertexBtn);
    vLayout->addWidget(removeVertexBtn);
    vertexBox->setLayout(vLayout);
    leftPanel->addWidget(vertexBox);

    // ---- Рёбра ----
    QGroupBox* edgeBox = new QGroupBox("Рёбра");
    QGridLayout* eLayout = new QGridLayout;
    eLayout->addWidget(new QLabel("From:"), 0, 0);
    fromEdit = new QLineEdit;
    eLayout->addWidget(fromEdit, 0, 1);
    eLayout->addWidget(new QLabel("To:"), 1, 0);
    toEdit = new QLineEdit;
    eLayout->addWidget(toEdit, 1, 1);
    eLayout->addWidget(new QLabel("Weight:"), 2, 0);
    weightEdit = new QLineEdit("1");
    eLayout->addWidget(weightEdit, 2, 1);
    QPushButton* addEdgeBtn = new QPushButton("Добавить ребро");
    QPushButton* removeEdgeBtn = new QPushButton("Удалить ребро");
    eLayout->addWidget(addEdgeBtn, 3, 0);
    eLayout->addWidget(removeEdgeBtn, 3, 1);
    edgeBox->setLayout(eLayout);
    leftPanel->addWidget(edgeBox);

    // ---- Матрица смежности ----
    QPushButton* editMatrixBtn = new QPushButton("Редактировать матрицу смежности");
    leftPanel->addWidget(editMatrixBtn);

    // ---- Алгоритмы TSP ----
    QGroupBox* tspBox = new QGroupBox("Задача коммивояжёра");
    QVBoxLayout* tspLayout = new QVBoxLayout;
    tspLayout->addWidget(new QLabel("Стартовая вершина (для Nearest Neighbor):"));
    startVertexCombo = new QComboBox;
    tspLayout->addWidget(startVertexCombo);
    QPushButton* branchBoundBtn = new QPushButton("Точный метод (ветви и границы)");
    QPushButton* nearestBtn = new QPushButton("Приближённый (ближайший сосед)");
    tspLayout->addWidget(branchBoundBtn);
    tspLayout->addWidget(nearestBtn);
    tspBox->setLayout(tspLayout);
    leftPanel->addWidget(tspBox);

    // ---- Вывод результатов ----
    outputEdit = new QTextEdit;
    outputEdit->setReadOnly(true);
    leftPanel->addWidget(outputEdit);

    // ---- Графический виджет ----
    graphWidget = new TSPWidget;
    graphWidget->setGraph(&graph);

    mainLayout->addLayout(leftPanel, 1);
    mainLayout->addWidget(graphWidget, 2);

    // Подключения сигналов
    connect(addVertexBtn, &QPushButton::clicked, this, &MainWindow::onAddVertex);
    connect(removeVertexBtn, &QPushButton::clicked, this, &MainWindow::onRemoveVertex);
    connect(addEdgeBtn, &QPushButton::clicked, this, &MainWindow::onAddEdge);
    connect(removeEdgeBtn, &QPushButton::clicked, this, &MainWindow::onRemoveEdge);
    connect(editMatrixBtn, &QPushButton::clicked, this, &MainWindow::onShowMatrix);
    connect(branchBoundBtn, &QPushButton::clicked, this, &MainWindow::onSolveBranchAndBound);
    connect(nearestBtn, &QPushButton::clicked, this, &MainWindow::onSolveNearestNeighbor);
    connect(graphWidget, &TSPWidget::vertexClicked, this, &MainWindow::onVertexSelected);

    refreshAll();
    updateStartVertexCombo();
}

MainWindow::~MainWindow() {}

void MainWindow::refreshAll() {
    graphWidget->update();
    updateStartVertexCombo();
}

void MainWindow::updateStartVertexCombo() {
    startVertexCombo->clear();
    for (int i = 0; i < graph.vertexCount(); ++i)
        startVertexCombo->addItem(QString::number(i));
}

void MainWindow::onAddVertex() {
    graph.addVertex();
    refreshAll();
    outputEdit->append("Добавлена вершина");
}

void MainWindow::onRemoveVertex() {
    int v = graphWidget->getSelectedVertex();
    if (v >= 0 && v < graph.vertexCount()) {
        graph.removeVertex(v);
        graphWidget->clearSelection();
        refreshAll();
        outputEdit->append(QString("Удалена вершина %1").arg(v));
    } else {
        QMessageBox::information(this, "Удаление", "Сначала выберите вершину на графике");
    }
}

void MainWindow::onAddEdge() {
    bool ok1, ok2, ok3;
    int from = fromEdit->text().toInt(&ok1);
    int to = toEdit->text().toInt(&ok2);
    int w = weightEdit->text().toInt(&ok3);
    if (ok1 && ok2 && ok3 && from >=0 && from < graph.vertexCount() &&
        to >=0 && to < graph.vertexCount()) {
        graph.addEdge(from, to, w);
        refreshAll();
        outputEdit->append(QString("Добавлено ребро %1-%2 вес %3").arg(from).arg(to).arg(w));
    } else {
        QMessageBox::warning(this, "Ошибка", "Некорректные данные");
    }
}

void MainWindow::onRemoveEdge() {
    bool ok1, ok2;
    int from = fromEdit->text().toInt(&ok1);
    int to = toEdit->text().toInt(&ok2);
    if (ok1 && ok2 && from >=0 && from < graph.vertexCount() && to >=0 && to < graph.vertexCount()) {
        graph.removeEdge(from, to);
        refreshAll();
        outputEdit->append(QString("Удалено ребро %1-%2").arg(from).arg(to));
    } else {
        QMessageBox::warning(this, "Ошибка", "Некорректные индексы");
    }
}

void MainWindow::onShowMatrix() {
    int n = graph.vertexCount();
    QDialog* dlg = new QDialog(this);
    dlg->setWindowTitle("Матрица смежности");
    QVBoxLayout* layout = new QVBoxLayout(dlg);
    QTableWidget* table = new QTableWidget(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            table->setItem(i, j, new QTableWidgetItem(QString::number(graph.getEdgeWeight(i,j))));
        }
    }
    layout->addWidget(table);
    QPushButton* okBtn = new QPushButton("Применить");
    layout->addWidget(okBtn);
    connect(okBtn, &QPushButton::clicked, [=](){
        QVector<QVector<int>> newMat(n, QVector<int>(n, 0));
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j)
                newMat[i][j] = table->item(i,j)->text().toInt();
        graph.setAdjMatrix(newMat);
        refreshAll();
        dlg->accept();
    });
    dlg->exec();
}

void MainWindow::displayResult(const TSPGraph::TSPResult& result, const QString& methodName) {
    if (!result.feasible) {
        outputEdit->append(methodName + ": Нет гамильтонова цикла (граф несвязный или нет рёбер)");
        graphWidget->setHighlightedPath(QList<int>());
        return;
    }
    QString pathStr;
    for (int v : result.path) pathStr += QString::number(v) + " ";
    outputEdit->append(methodName + ": путь = " + pathStr + " | длина = " + QString::number(result.totalCost));
    graphWidget->setHighlightedPath(result.path);
}

void MainWindow::onSolveBranchAndBound() {
    if (graph.vertexCount() < 2) {
        outputEdit->append("Точный метод: слишком мало вершин");
        return;
    }
    TSPGraph::TSPResult res = graph.solveBranchAndBound();
    displayResult(res, "Точный метод (ветви и границы)");
}

void MainWindow::onSolveNearestNeighbor() {
    if (graph.vertexCount() < 2) {
        outputEdit->append("Приближённый метод: слишком мало вершин");
        return;
    }
    int start = startVertexCombo->currentIndex();
    TSPGraph::TSPResult res = graph.solveNearestNeighbor(start);
    displayResult(res, QString("Приближённый метод (старт %1)").arg(start));
}

void MainWindow::onVertexSelected(int v) {
    outputEdit->append(QString("Выбрана вершина %1").arg(v));
}
