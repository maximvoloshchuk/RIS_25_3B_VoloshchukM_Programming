#include "tspgraph.h"
#include <queue>
#include <algorithm>

TSPGraph::TSPGraph(int initialSize) {
    if (initialSize > 0) {
        adjMatrix.resize(initialSize);
        for (int i = 0; i < initialSize; ++i)
            adjMatrix[i] = QVector<int>(initialSize, 0);
    }
}

void TSPGraph::addVertex() {
    int n = vertexCount();
    adjMatrix.resize(n + 1);
    for (int i = 0; i <= n; ++i) {
        adjMatrix[i].resize(n + 1);
        if (i == n) {
            adjMatrix[i] = QVector<int>(n + 1, 0);
        } else {
            adjMatrix[i][n] = 0;
        }
    }
}

void TSPGraph::removeVertex(int v) {
    if (v < 0 || v >= vertexCount()) return;
    adjMatrix.erase(adjMatrix.begin() + v);
    for (int i = 0; i < adjMatrix.size(); ++i) {
        adjMatrix[i].erase(adjMatrix[i].begin() + v);
    }
}

void TSPGraph::addEdge(int from, int to, int weight) {
    if (from < 0 || from >= vertexCount() || to < 0 || to >= vertexCount()) return;
    if (weight <= 0) weight = 1;
    adjMatrix[from][to] = weight;
    adjMatrix[to][from] = weight;
}

void TSPGraph::removeEdge(int from, int to) {
    if (from < 0 || from >= vertexCount() || to < 0 || to >= vertexCount()) return;
    adjMatrix[from][to] = 0;
    adjMatrix[to][from] = 0;
}

void TSPGraph::setEdgeWeight(int from, int to, int weight) {
    addEdge(from, to, weight);
}

int TSPGraph::getEdgeWeight(int from, int to) const {
    if (from < 0 || from >= vertexCount() || to < 0 || to >= vertexCount()) return 0;
    return adjMatrix[from][to];
}

QVector<QVector<int>> TSPGraph::getAdjMatrix() const {
    return adjMatrix;
}

void TSPGraph::setAdjMatrix(const QVector<QVector<int>>& matrix) {
    if (matrix.isEmpty()) return;
    int n = matrix.size();
    adjMatrix = matrix;
    for (int i = 0; i < n; ++i)
        for (int j = i+1; j < n; ++j)
            if (adjMatrix[i][j] != adjMatrix[j][i])
                adjMatrix[j][i] = adjMatrix[i][j];
}

// ---------- Branch and Bound ----------
int TSPGraph::lowerBound(int visitedMask, int currentVertex) const {
    // Простая оценка: сумма минимальных рёбер из каждой непосещённой вершины
    int n = vertexCount();
    int lb = 0;
    for (int v = 0; v < n; ++v) {
        if (v == currentVertex || (visitedMask & (1 << v))) continue;
        int minEdge = INF;
        for (int u = 0; u < n; ++u) {
            if (u == v) continue;
            int w = adjMatrix[v][u];
            if (w > 0 && w < minEdge) minEdge = w;
        }
        if (minEdge == INF) return INF; // нет пути
        lb += minEdge;
    }
    // Добавляем минимальное ребро из currentVertex в любую непосещённую
    int minFromCurrent = INF;
    for (int u = 0; u < n; ++u) {
        if (u != currentVertex && !(visitedMask & (1 << u)) && adjMatrix[currentVertex][u] > 0) {
            if (adjMatrix[currentVertex][u] < minFromCurrent)
                minFromCurrent = adjMatrix[currentVertex][u];
        }
    }
    if (minFromCurrent == INF) return INF;
    lb += minFromCurrent;
    return lb;
}

void TSPGraph::branchAndBoundRec(int currentVertex, int visitedMask, int cost,
                                  QList<int>& currentPath, TSPResult& best) {
    int n = vertexCount();
    if (currentPath.size() == n) {
        // Замкнули цикл: добавим ребро обратно в начало
        int first = currentPath.first();
        int backCost = adjMatrix[currentVertex][first];
        if (backCost > 0 && cost + backCost < best.totalCost) {
            best.totalCost = cost + backCost;
            best.path = currentPath;
            best.path.append(first);
            best.feasible = true;
        }
        return;
    }

    // Перебор всех непосещённых вершин
    for (int next = 0; next < n; ++next) {
        if (visitedMask & (1 << next)) continue;
        int edgeCost = adjMatrix[currentVertex][next];
        if (edgeCost == 0) continue; // нет ребра
        int newCost = cost + edgeCost;
        if (newCost >= best.totalCost) continue; // отсечение

        // Оценка нижней границы
        int bound = lowerBound(visitedMask | (1 << next), next);
        if (newCost + bound >= best.totalCost) continue;

        currentPath.append(next);
        branchAndBoundRec(next, visitedMask | (1 << next), newCost, currentPath, best);
        currentPath.removeLast();
    }
}

TSPGraph::TSPResult TSPGraph::solveBranchAndBound() {
    TSPResult best;
    best.totalCost = INF;
    best.feasible = false;
    int n = vertexCount();
    if (n == 0) return best;

    // Начинаем с каждой вершины (симметрично, можно с 0)
    QList<int> startPath;
    startPath.append(0);
    branchAndBoundRec(0, 1 << 0, 0, startPath, best);
    return best;
}

// ---------- Nearest Neighbor ----------
TSPGraph::TSPResult TSPGraph::solveNearestNeighbor(int start) {
    TSPResult result;
    result.totalCost = 0;
    result.feasible = true;
    int n = vertexCount();
    if (n == 0 || start < 0 || start >= n) {
        result.feasible = false;
        return result;
    }

    QVector<bool> visited(n, false);
    QList<int> path;
    int current = start;
    path.append(current);
    visited[current] = true;

    for (int step = 1; step < n; ++step) {
        int nearest = -1;
        int minDist = INF;
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && adjMatrix[current][v] > 0 && adjMatrix[current][v] < minDist) {
                minDist = adjMatrix[current][v];
                nearest = v;
            }
        }
        if (nearest == -1) {
            result.feasible = false;
            return result;
        }
        result.totalCost += minDist;
        path.append(nearest);
        visited[nearest] = true;
        current = nearest;
    }
    // Возврат в стартовую вершину
    int backCost = adjMatrix[current][start];
    if (backCost == 0) {
        result.feasible = false;
        return result;
    }
    result.totalCost += backCost;
    path.append(start);
    result.path = path;
    return result;
}
