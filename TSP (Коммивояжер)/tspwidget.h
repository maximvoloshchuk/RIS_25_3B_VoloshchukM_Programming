#ifndef TSPWIDGET_H
#define TSPWIDGET_H

#include <QWidget>
#include <QMap>
#include <QPoint>
#include "tspgraph.h"

class TSPWidget : public QWidget {
    Q_OBJECT
public:
    explicit TSPWidget(QWidget *parent = nullptr);
    void setGraph(TSPGraph* graph);
    void setHighlightedPath(const QList<int>& path);  // отобразить найденный маршрут
    void setSelectedVertex(int v);
    void clearSelection();

    int getSelectedVertex() const { return m_selectedVertex; }

signals:
    void vertexClicked(int v);
    void vertexMoved(int v, const QPoint& newPos);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    TSPGraph* m_graph;
    QMap<int, QPoint> m_vertexPositions;
    QList<int> m_highlightedPath;
    int m_selectedVertex;
    QPoint m_dragStartPos;
    bool m_dragging;

    void computeInitialPositions();
    int findVertexAtPoint(const QPoint& p) const;
    void drawEdge(QPainter& painter, int from, int to, int weight, bool highlight);
};

#endif // TSPWIDGET_H
