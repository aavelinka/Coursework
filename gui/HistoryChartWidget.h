#ifndef HISTORYCHARTWIDGET_H
#define HISTORYCHARTWIDGET_H

#include <QWidget>
#include <vector>
#include "user/BodyMeasurement.h"

class HistoryChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HistoryChartWidget(QWidget* parent = nullptr);
    void setMeasurements(const std::vector<Measurement>& data);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;

private:
    std::vector<Measurement> measurements;
    std::vector<QPointF> points;
    int hoverIndex{-1};
};

#endif
