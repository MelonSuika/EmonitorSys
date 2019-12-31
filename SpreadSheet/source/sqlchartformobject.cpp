#include "sqlchartFormobject.h"

SQLChartFormObject::SQLChartFormObject()
{
    //QSharedPointer<QCPGraphDataContainer>
    //qRegisterMetaType();
    //m_dataDensity_Time = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    //m_dataPressure_Time = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);
    //m_dataTemperature_Time = QSharedPointer<QCPGraphDataContainer>(new QCPGraphDataContainer);

    m_vctDDate = new QVector<double>;
    m_vctTemperature = new QVector<double>;
    m_vctDensity = new QVector<double>;
    m_vctPressure = new QVector<double>;
}

void SQLChartFormObject::readData(QDateTime start, QDateTime end)
{
    /* 建立并打开数据库 */
    QSqlDatabase dc = QSqlDatabase::addDatabase( "QSQLITE" , "cn2");
    dc.setDatabaseName( "test.db");

    if (!dc.open())
    {
        LOG(ERROR)<<"Error: Failed to connect database." << dc.lastError().text().toStdString();
        return ;
    }
    else
    {
        LOG(INFO) << "Succeed to connect database.";
    }

    m_query = new QSqlQuery(dc);
    int n = 0;
    bool isScd = false;
    isScd = m_query->exec(QString("SELECT * FROM TH015 WHERE time > '%1' and time < '%2';").
                          arg(start.toString("yyyy-MM-dd hh:mm")).arg(end.toString("yyyy-MM-dd hh:mm")));

    qDebug()<<"exec"<<(isScd?"success":"fail");

    m_vctDDate->clear();
    m_vctDensity->clear();
    m_vctPressure->clear();
    m_vctTemperature->clear();
    while(m_query->next())
    {
        double time = QDateTime::fromString(m_query->value(0).toString(), "yyyy-MM-dd hh:mm:ss").toSecsSinceEpoch();
        double t = m_query->value(3).toDouble();
        double d = m_query->value(2).toDouble();
        double p = m_query->value(1).toDouble();
        m_vctDDate->append(time);
        m_vctDensity->append(d);
        m_vctPressure->append(p);
        m_vctTemperature->append(t);
        //m_dataDensity_Time->add(QCPGraphData(d, time));
        //m_dataPressure_Time->add(QCPGraphData(p, time));
        //m_dataTemperature_Time->add(QCPGraphData(t, time));

        n++;
        if(n > 200000)
            break;
    }

    emit loadOver(m_vctDensity, m_vctPressure, m_vctTemperature, m_vctDDate);
    qDebug()<<"over";

}
