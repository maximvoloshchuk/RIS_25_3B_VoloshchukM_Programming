#include "tspwidget.h"
#include <QPainter>
#include <QMouseEvent>
#include <QtMath>
#include <QPen>

TSPWidget::TSPWidget(QWidget *parent)
    : QWidget(parent), m_graph(nullptr), m_selectedVertex(-1), m_dragging(false)
{
    setMinimumSize(500, 400);
}

void TSPWidget::setGraph(TSPGraph* graph) {
    m_graph = graph;
    computeInitialPositions();
    update();
}

void TSPWidget::setHighlightedPath(const QList<int>& path) {
    m_highlightedPath = path;
    update();
}

void TSPWidget::setSelectedVertex(int v) {
    m_selectedVertex = v;
    update();
}

void TSPWidget::clearSelection() {
    m_selectedVertex = -1;
    update();
}

void TSPWidget::computeInitialPositions() {
    if (!m_graph || m_graph->vertexCount() == 0) return;
    int n = m_graph->vertexCount();
    double angleStep = 2 * M_PI / n;
    QRect rect = this->rect().adjusted(50, 50, -50, -50);
    QPoint center = rect.center();
    int radius = qMin(rect.width(), rect.height()) / 2;
    for (int i = 0; i < n; ++i) {
        double angle = i * angleStep;
        int x = center.x() + radius * qCos(angle);
        int y = center.y() + radius * qSin(angle);
        m_vertexPositions[i] = QPoint(x, y);
    }
}

void TSPWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    if (!m_graph || m_graph->vertexCount() == 0) return;
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    // Рисуем все рёбра (обычным цветом)
    int n = m_graph->vertexCount();
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            int w = m_graph->getEdgeWeight(i, j);
            if (w != 0) {
                bool highlight = false;
                // Проверяем, входит ли ребро в подсвеченный маршрут
                if (m_highlightedPath.size() > 1) {
                    for (int k = 0; k < m_highlightedPath.size() - 1; ++k) {
                        if ((m_highlightedPath[k] == i && m_highlightedPath[k+1] == j) ||
                            (m_highlightedPath[k] == j && m_highlightedPath[k+1] == i)) {
                            highlight = true;
                            break;
                        }
                    }
                }
                drawEdge(painter, i, j, w, highlight);
            }
        }
    }

    // Рисуем вершины
    for (int i = 0; i < n; ++i) {
        QPoint p = m_vertexPositions[i];
        painter.setBrush(i == m_selectedVertex ? Qt::green : Qt::lightGray);
        painter.setPen(Qt::black);
        painter.drawEllipse(p, 20, 20);
        painter.drawText(QRect(p.x()-15, p.y()-15, 30, 30), Qt::AlignCenter, QString::number(i));
    }
}

void TSPWidget::drawEdge(QPainter& painter, int from, int to, int weight, bool highlight) {
    QPoint p1 = m_vertexPositions[from];
    QPoint p2 = m_vertexPositions[to];
    if (highlight) {
        QPen pen(Qt::red, 3);
        painter.setPen(pen);
    } else {
        painter.setPen(QPen(Qt::black, 1));
    }
    painter.drawLine(p1, p2);
    QPoint mid = (p1 + p2) / 2;
    painter.setPen(Qt::darkGray);
    painter.drawText(mid + QPoint(5, -5), QString::number(weight));
}

int TSPWidget::findVertexAtPoint(const QPoint& p) const {
    for (auto it = m_vertexPositions.constBegin(); it != m_vertexPositions.constEnd(); ++it) {
        if ((p - it.value()).manhattanLength() < 20)
            return it.key();
    }
    return -1;
}

void TSPWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        int v = findVertexAtPoint(event->pos());
        if (v != -1) {
            m_selectedVertex = v;
            m_dragging = true;
            m_dragStartPos = event->pos();
            emit vertexClicked(v);
            update();
        }
    }
}

void TSPWidget::mouseMoveEvent(QMouseEvent *event) {
    if (m_dragging && m_selectedVertex != -1) {
        m_vertexPositions[m_selectedVertex] = event->pos();
        update();
        emit vertexMoved(m_selectedVertex, event->pos());
    }
}

void TSPWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_dragging) {
        m_dragging = false;
    }
}
