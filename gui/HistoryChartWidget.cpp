#include "HistoryChartWidget.h"
#include <QPainter>
#include <QLinearGradient>
#include <QPen>
#include <QMouseEvent>
#include <QToolTip>
#include <algorithm>
#include <sstream>

HistoryChartWidget::HistoryChartWidget(QWidget* parent)
    : QWidget(parent)
{
    setMinimumHeight(220);
    setMouseTracking(true);
}

void HistoryChartWidget::setMeasurements(const std::vector<Measurement>& data)
{
    measurements = data;
    std::sort(measurements.begin(), measurements.end(), [](const Measurement& a, const Measurement& b) {
        return std::tie(a.date.year, a.date.month, a.date.day) <
               std::tie(b.date.year, b.date.month, b.date.day);
    });
    update();
}

void HistoryChartWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    QRectF area = rect().adjusted(12, 12, -12, -12);
    points.clear();
    hoverIndex = std::min(hoverIndex, measurements.size() > 0 ? (int)measurements.size() - 1 : -1);

    QLinearGradient bg(area.topLeft(), area.bottomLeft());
    bg.setColorAt(0.0, QColor(15, 47, 47, 20));
    bg.setColorAt(1.0, QColor(15, 47, 47, 8));
    p.fillRect(area, bg);

    if (measurements.size() < 2)
    {
        p.setPen(QPen(QColor(15, 47, 47, 180), 1));
        p.drawText(area, Qt::AlignCenter, "Недостаточно данных для графика");
        return;
    }

    double minWeight = measurements.front().weight;
    double maxWeight = measurements.front().weight;
    for (const auto& m : measurements)
    {
        minWeight = std::min(minWeight, m.weight);
        maxWeight = std::max(maxWeight, m.weight);
    }
    if (maxWeight - minWeight < 0.1)
    {
        maxWeight = minWeight + 1.0;
    }

    const int n = static_cast<int>(measurements.size());
    points.reserve(n);
    for (int i = 0; i < n; ++i)
    {
        double x = area.left() + (area.width() * i) / std::max(1, n - 1);
        double norm = (measurements[i].weight - minWeight) / (maxWeight - minWeight);
        double y = area.bottom() - norm * area.height();
        points.emplace_back(x, y);
    }

    // axes
    p.setPen(QPen(QColor(15, 47, 47, 50), 1));
    p.drawLine(area.bottomLeft(), area.bottomRight());
    p.drawLine(area.bottomLeft(), area.topLeft());

    // line
    QPen linePen(QColor(15, 47, 47, 180), 2);
    linePen.setCapStyle(Qt::RoundCap);
    p.setPen(linePen);
    for (int i = 1; i < points.size(); ++i)
    {
        p.drawLine(points[i - 1], points[i]);
    }

    // points
    p.setBrush(QColor(15, 47, 47, 200));
    for (const auto& pt : points)
    {
        p.drawEllipse(pt, 4, 4);
    }

    // labels
    p.setPen(QPen(QColor(15, 47, 47, 160), 1));
    QString minLabel = QString::number(minWeight, 'f', 1) + " кг";
    QString maxLabel = QString::number(maxWeight, 'f', 1) + " кг";
    p.drawText(area.adjusted(0, 0, -4, 0), Qt::AlignRight | Qt::AlignTop, maxLabel);
    p.drawText(area.adjusted(0, 0, -4, -4), Qt::AlignRight | Qt::AlignBottom, minLabel);

    // hover label
    if (hoverIndex >= 0 && hoverIndex < points.size())
    {
        QPointF pt = points[hoverIndex];
        const Measurement& m = measurements[hoverIndex];
        QString dateStr = QString("%1-%2-%3")
            .arg(m.date.year)
            .arg(m.date.month, 2, 10, QChar('0'))
            .arg(m.date.day, 2, 10, QChar('0'));
        QString label = QString("%1 кг · %2").arg(m.weight, 0, 'f', 1).arg(dateStr);

        const int margin = 8;
        QFontMetrics fm(p.font());
        qreal bubbleW = fm.horizontalAdvance(label) + 14;
        qreal bubbleH = fm.height() + 10;
        qreal bx = pt.x() + 10;
        qreal by = pt.y() - bubbleH - 6;

        if (bx + bubbleW > rect().right() - margin)
        {
            bx = pt.x() - bubbleW - 10;
        }
        if (bx < rect().left() + margin)
        {
            bx = rect().left() + margin;
        }
        if (by < rect().top() + margin)
        {
            by = pt.y() + 10;
        }
        QRectF bubble(bx, by, bubbleW, bubbleH);

        p.setBrush(QColor(15, 47, 47, 220));
        p.setPen(Qt::NoPen);
        p.drawRoundedRect(bubble, 6, 6);
        p.setPen(QPen(QColor("#f8f1d4")));
        p.drawText(bubble.adjusted(6, 0, -6, 0), Qt::AlignVCenter | Qt::AlignLeft, label);
        p.setBrush(QColor(15, 47, 47, 220));
        p.setPen(QPen(QColor("#f8f1d4")));
        p.drawEllipse(pt, 6, 6);
    }
}

void HistoryChartWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (points.empty())
    {
        return;
    }
    int closest = -1;
    qreal bestDist = 1e9;
    for (int i = 0; i < points.size(); ++i)
    {
        qreal dist = QLineF(event->position(), points[i]).length();
        if (dist < bestDist)
        {
            bestDist = dist;
            closest = i;
        }
    }
    if (closest != -1 && bestDist < 18.0 && closest != hoverIndex)
    {
        hoverIndex = closest;
        const Measurement& m = measurements[closest];
        QString dateStr = QString("%1-%2-%3")
            .arg(m.date.year)
            .arg(m.date.month, 2, 10, QChar('0'))
            .arg(m.date.day, 2, 10, QChar('0'));
        QString tip = QString("%1 кг\n%2").arg(m.weight, 0, 'f', 1).arg(dateStr);
        QToolTip::showText(event->globalPosition().toPoint(), tip, this);
        update();
    }
    else if (bestDist >= 18.0 && hoverIndex != -1)
    {
        hoverIndex = -1;
        QToolTip::hideText();
        update();
    }
}

void HistoryChartWidget::leaveEvent(QEvent* event)
{
    Q_UNUSED(event);
    hoverIndex = -1;
    QToolTip::hideText();
    update();
}
