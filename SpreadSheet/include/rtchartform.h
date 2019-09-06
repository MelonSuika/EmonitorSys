#ifndef RTCHARTFORM_H
#define RTCHARTFORM_H

#include <QWidget>
#include <QtCharts>
#include <QtWidgets/QGraphicsView>
#include <QtCharts/QChartGlobal>
#include "callout.h"

using namespace QtCharts;
namespace Ui {
class RtChartForm;
}

class RtChartForm : public QWidget
{
    Q_OBJECT

public:
    explicit RtChartForm(QWidget *parent = nullptr);
    ~RtChartForm();

private:
    Ui::RtChartForm *ui;

private slots:
    void rcvRtData(QJsonObject *, int);

    void on_pushButton_temperature_clicked();

    void on_pushButton_density_clicked();

    void on_pushButton_allDisplay_clicked();

    void on_pushButton_pressure_clicked();

private:
    int m_nMaxSize;
    int m_nMaxX;
    int m_nMaxY;

    int m_nXIndex;

    QChart *m_Chart;
    QChartView *m_ChartView;
    QLineSeries *m_Series;
    QLineSeries *m_SeriesDensity;
    QSplineSeries *m_SeriesTemperature;

    QValueAxis *m_axnisX;
    QValueAxis *m_axnisY;
    QValueAxis *m_axnisYDensity;
    QValueAxis *m_axnisYTemperature;
    QCategoryAxis *m_Y;
    QDateTimeAxis *m_dateTimeAxisX;


protected:
    //void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void keepCallout();
    void tooltip(QPointF point, bool state);
    public:
    QGraphicsSimpleTextItem *m_coordX;
    QGraphicsSimpleTextItem *m_coordY;
    Callout *m_tooltip;
    QList<Callout *> m_callouts;


};

#endif // RTCHARTFORM_H
