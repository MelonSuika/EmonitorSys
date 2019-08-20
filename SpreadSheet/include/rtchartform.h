#ifndef RTCHARTFORM_H
#define RTCHARTFORM_H

#include <QWidget>
#include <QtCharts>

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


    int m_nMaxSize;
    int m_nMaxX;
    int m_nMaxY;

    QChart *m_Chart;
    QChartView *m_ChartView;
    QLineSeries *m_Series;
    QValueAxis *m_axnisX;
    QValueAxis *m_axnisY;


};

#endif // RTCHARTFORM_H
