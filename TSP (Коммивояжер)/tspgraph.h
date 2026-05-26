#ifndef TSPGRAPH_H
#define TSPGRAPH_H

#include <QVector>
#include <QList>
#include <QPair>
#include <limits>

class TSPGraph {
public:
    TSPGraph(int initialSize = 0);

    // Редактирование графа
    void addVertex();
    void removeVertex(int v);
    void addEdge(int from, int to, int weight);
    void removeEdge(int from, int to);
    void setEdgeWeight(int from, int to, int weight);
    int getEdgeWeight(int from, int to) const;

    // Получение матрицы смежности
    QVector<QVector<int>> getAdjMatrix() const;
    void setAdjMatrix(const QVector<QVector<int>>& matrix);

    // Информация о графе
    int vertexCount() const { return adjMatrix.size(); }
    bool isEmpty() const { return vertexCount() == 0; }

    // Алгоритмы TSP
    struct TSPResult {
        QList<int> path;   // порядок вершин (цикл, первая вершина повторена в конце)
        int totalCost;
        bool feasible;
    };

    TSPResult solveBranchAndBound();           // точный метод (ветви и границы)
    TSPResult solveNearestNeighbor(int start); // приближённый метод

    // Вспомогательные константы
    static const int INF = std::numeric_limits<int>::max() / 2;

private:
    QVector<QVector<int>> adjMatrix;

    // Рекурсивная функция для Branch and Bound
    void branchAndBoundRec(int currentVertex, int visitedMask, int cost,
                           QList<int>& currentPath, TSPResult& best);
    int lowerBound(int visitedMask, int currentVertex) const;
};

#endif // TSPGRAPH_H
