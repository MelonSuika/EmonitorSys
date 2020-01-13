#ifndef SQLCHARTFORMOBJECT_H
#define SQLCHARTFORMOBJECT_H

#include "common.h"
#include "qcustomplot.h"
class SQLChartFormObject : public QObject
{
    Q_OBJECT
public:
    SQLChartFormObject();

private slots:
    void readData(QDateTime start, QDateTime end);
private:
    QSqlQueryModel *m_model;
    QSqlQuery *m_query;
    QVector<double> m_dataDensity_Time;
    QVector<double> m_dataTemperature_Time;
    QVector<double> m_dataPressure_Time;
    QVector<double> *m_vctDDate;
    QVector<double> *m_vctTemperature;
    QVector<double> *m_vctDensity;
    QVector<double> *m_vctPressure;
signals:
    void loadOver(QVector<double>*, QVector<double>*, QVector<double>*, QVector<double>*);
};

#endif // SQLCHARTFORMOBJECT_H
