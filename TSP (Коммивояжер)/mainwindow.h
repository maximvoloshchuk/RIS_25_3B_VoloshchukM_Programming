#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include "tspgraph.h"
#include "tspwidget.h"

class QLineEdit;
class QComboBox;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddVertex();
    void onRemoveVertex();
    void onAddEdge();
    void onRemoveEdge();
    void onShowMatrix();
    void onSolveBranchAndBound();
    void onSolveNearestNeighbor();
    void onVertexSelected(int v);

private:
    TSPGraph graph;
    TSPWidget* graphWidget;
    QLineEdit* fromEdit;
    QLineEdit* toEdit;
    QLineEdit* weightEdit;
    QComboBox* startVertexCombo;
    QTextEdit* outputEdit;

    void refreshAll();
    void updateStartVertexCombo();
    void displayResult(const TSPGraph::TSPResult& result, const QString& methodName);
};

#endif // MAINWINDOW_H
