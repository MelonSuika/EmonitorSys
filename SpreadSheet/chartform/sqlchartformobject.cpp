#include "sqlchartFormobject.h"

SQLChartFormObject::SQLChartFormObject()
{
    m_vctDDate = new QVector<double>;
    m_vctTemperature = new QVector<double>;
    m_vctDensity = new QVector<double>;
    m_vctPressure = new QVector<double>;
}


/*
    函数功能:读数据库
*/
void SQLChartFormObject::readData(QDateTime start, QDateTime end, unsigned int u32Addr)
{
    /* 建立并打开数据库 */
    QSqlDatabase dc = QSqlDatabase::database("cn3");
    if (!dc.isOpen())
    {
        dc = QSqlDatabase::addDatabase( "QSQLITE" , "cn3");
        dc.setDatabaseName( DATABASENAME);
    }

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
    if (u32Addr == 0)
    {
        isScd = m_query->exec(QString("SELECT time, pressure, density, temperature FROM TH015A WHERE time > '%1' and time < '%2';").
                            arg(start.toString("yyyy-MM-dd hh:mm")).arg(end.toString("yyyy-MM-dd hh:mm")));
    }
    else
    {
        isScd = m_query->exec(QString("SELECT time, pressure, density, temperature FROM TH015A WHERE time > '%1' and time < '%2' and address == '%3';").
                            arg(start.toString("yyyy-MM-dd hh:mm")).arg(end.toString("yyyy-MM-dd hh:mm")).arg(u32Addr));
    }

    qDebug()<<"exec"<<(isScd?"success":"fail");

    m_vctDDate->clear();
    m_vctDDate->squeeze();
    m_vctDensity->clear();
    m_vctDensity->squeeze();
    m_vctPressure->clear();
    m_vctPressure->squeeze();
    m_vctTemperature->clear();
    m_vctTemperature->squeeze();

    while(m_query->next())
    {
        double time = QDateTime::fromString(m_query->value(0).toString(), "yyyy-MM-dd hh:mm:ss.z").toSecsSinceEpoch();
        double t = m_query->value(3).toDouble();
        double d = m_query->value(2).toDouble();
        double p = m_query->value(1).toDouble();
        m_vctDDate->append(time);
        m_vctDensity->append(d);
        m_vctPressure->append(p);
        m_vctTemperature->append(t);

        n++;
        if(n > 86400*7)
            break;
    }

    emit loadOver(m_vctDensity, m_vctPressure, m_vctTemperature, m_vctDDate);
    qDebug()<<"over";

    delete m_query;

}
