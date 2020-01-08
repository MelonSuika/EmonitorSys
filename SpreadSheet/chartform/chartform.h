#ifndef CHARTFORM_H
#define CHARTFORM_H

#include <QWidget>
#include "common.h"
#include <QDateTime>
#include "rtchartform.h"
#include "qcustomplot.h"
#include "sqlchartformobject.h"

namespace Ui {
class ChartForm;
}

class ChartForm : public QWidget
{
    Q_OBJECT

public:
    explicit ChartForm(QWidget *parent = nullptr);
    ~ChartForm();

private slots:

    void on_pushButton_display_clicked();

    void on_dateTimeEdit_start_dateTimeChanged(const QDateTime &dateTime);

    void on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime);

    void on_pushButton_rtDisplay_clicked();

    void on_pushButton_testAddData_clicked();

    void disPlay(QVector<double>*,QVector<double>*,QVector<double>*,QVector<double>*);

    void custPlotMoveEvent(QMouseEvent*);
signals:
    void onLoad(QDateTime, QDateTime);
private:
    Ui::ChartForm *ui;

    QDateTime m_startDt;
    QDateTime m_endDt;

    QChart *m_chart;
    QChartView *m_chartView;
    QSplineSeries *m_series;
    QVector<QPointF> *m_vectorPoint;
    QVector<QDateTime> *m_vctDateTime;
    QVector<double> *m_vctDDate;
    QVector<double> *m_vctTemperature;
    QVector<double> *m_vctDensity;
    QVector<double> *m_vctPressure;
    QVector<QCPGraphData> *m_dataDensity_Time;
    QVector<QCPGraphData> *m_dataTemperature_Time;
    QVector<QCPGraphData> *m_dataPressure_Time;
    int m_nEnd;

    SQLChartFormObject *m_sqlChartFormObj;

    /* 数据处理线程 */
    QPointer<QThread> m_sqlProcess;
    /* 耗时统计 */
    QTime m_timeConsume;
};

#endif // CHARTFORM_H
