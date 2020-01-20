#include "sqlhistoryanalysisobject.h"

SqlHistoryAnalysisObject::SqlHistoryAnalysisObject()
{

}


void SqlHistoryAnalysisObject::readData(TransInfo info)
{

    /* 打开数据库连接 */
    QSqlDatabase dc = QSqlDatabase::database("cnHst");
    if (!dc.isOpen())
    {
        dc = QSqlDatabase::addDatabase( "QSQLITE" , "cnHst");
        dc.setDatabaseName( DATABASENAME);
        LOG(INFO)<<"Add database connect cnHst";
    }

    if (!dc.open())
    {
        LOG(ERROR)<<"Error: cnHst Failed to connect database." << dc.lastError().text().toStdString();
        return ;
    }
    else
    {
        LOG(INFO) << "cnHst Succeed to connect database.";
    }

    m_query = new QSqlQuery(dc);
    int n = 0;

    /*while(m_query->next())
    {
        double time = QDateTime::fromString(m_query->value(0).toString(), "yyyy-MM-dd hh:mm:ss.z").toSecsSinceEpoch();
        double t = m_query->value(3).toDouble();
        double d = m_query->value(2).toDouble();
        double p = m_query->value(1).toDouble();

        n++;
        if(n > 86400*7)
            break;
    }*/
    TSheetData sheetData;
    /* 分开查 */
#if 0
    m_query->exec(QString("select avg(density) from TH015A where time > '%1' and time < '%2'").
                                         arg(info.startTime.toString("yyyy-MM-dd hh:mm")).arg(info.endTime.toString("yyyy-MM-dd hh:mm")));
    m_query->next();
    sheetData.avgDensity = m_query->value(0).toDouble();
    m_query->exec(QString("select max(density) from TH015A where time > '%1' and time < '%2'").
                                         arg(info.startTime.toString("yyyy-MM-dd hh:mm")).arg(info.endTime.toString("yyyy-MM-dd hh:mm")));
    m_query->next();
    sheetData.maxDensity = m_query->value(0).toDouble();
    m_query->exec(QString("select min(density) from TH015A where time > '%1' and time < '%2'").
                                         arg(info.startTime.toString("yyyy-MM-dd hh:mm")).arg(info.endTime.toString("yyyy-MM-dd hh:mm")));
    m_query->next();
    sheetData.minDensity = m_query->value(0).toDouble();
#else
    /* 一起查 */
    m_query->exec(QString("select avg(density),max(density),min(density),avg(pressure),max(pressure),min(pressure),avg(temperature),max(temperature),min(temperature) from TH015A where time > '%1' and time < '%2'").
                                         arg(info.startTime.toString("yyyy-MM-dd hh:mm")).arg(info.endTime.toString("yyyy-MM-dd hh:mm")));
    m_query->next();

    sheetData.data[0][0] = m_query->value(0).toDouble();
    sheetData.data[0][1] = m_query->value(1).toDouble();
    sheetData.data[0][2] = m_query->value(2).toDouble();
    sheetData.data[0][3] = m_query->value(3).toDouble();
    sheetData.data[0][4] = m_query->value(4).toDouble();
    sheetData.data[0][5] = m_query->value(5).toDouble();
    sheetData.data[0][6] = m_query->value(6).toDouble();
    sheetData.data[0][7] = m_query->value(7).toDouble();
    sheetData.data[0][8] = m_query->value(8).toDouble();

    for (int i = 1; i <= 255; i++)
    {
        m_query->exec(QString("select count(*) from (select * from TH015A where time > '%1' and time < '%2' limit 0, 255) where address == '%3';").
                      arg(info.startTime.toString("yyyy-MM-dd hh:mm")).arg(info.endTime.toString("yyyy-MM-dd hh:mm")).arg(i));

        m_query->next();
        if (m_query->value(0).toDouble() == 0)
        {
            sheetData.data[i][0] = -1;
        }
        else {
            sheetData.data[i][0] = i;
        }

    }
    for (int i = 1; i <= 255; i++)
    {
        if (sheetData.data[i][0] == -1)
        {
            continue;
        }
        m_query->exec(QString("select avg(density),max(density),min(density),avg(pressure),max(pressure),min(pressure),avg(temperature),max(temperature),min(temperature) from TH015A where time > '%1' and time < '%2' and address == '%3'").
                                             arg(info.startTime.toString("yyyy-MM-dd hh:mm")).arg(info.endTime.toString("yyyy-MM-dd hh:mm")).arg(i));
        m_query->next();
        sheetData.data[i][0] = m_query->value(0).toDouble();
        sheetData.data[i][1] = m_query->value(1).toDouble();
        sheetData.data[i][2] = m_query->value(2).toDouble();
        sheetData.data[i][3] = m_query->value(3).toDouble();
        sheetData.data[i][4] = m_query->value(4).toDouble();
        sheetData.data[i][5] = m_query->value(5).toDouble();
        sheetData.data[i][6] = m_query->value(6).toDouble();
        sheetData.data[i][7] = m_query->value(7).toDouble();
        sheetData.data[i][8] = m_query->value(8).toDouble();

    }
#endif
    emit loadOver(sheetData);
}
