#ifndef CHARTFORM_H
#define CHARTFORM_H

#include <QWidget>
#include "common.h"
#include <QDateTime>


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

private:
    Ui::ChartForm *ui;

    QDateTime m_startDt;
    QDateTime m_endDt;

    QChart *m_chart;
    QChartView *m_chartView;
    QSplineSeries *m_series;
    QVector<QPointF> *m_vectorPoint;
    int m_nEnd;

    QSqlQueryModel *m_model;
    QSqlQuery *m_query;
};

#endif // CHARTFORM_H
