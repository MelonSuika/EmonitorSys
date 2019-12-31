#include "mainwindow.h"
#include <QApplication>
#include "connection.h"
#include <QProcess>
#include <QStyleFactory>
#include <QSqlError>
#include <QSqlQuery>

using namespace GOOGLE_NAMESPACE;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* 打开日志 */
    InitGoogleLogging(argv[0]);
    FLAGS_log_dir = "./";
    LOG(INFO)<<"Programe Run";

    /* 添加一个连接名为cnct1的SQLITE数据库连接 */
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE");
    db.setDatabaseName( "test.db" );

    if (!db.open())
    {
        LOG(ERROR)<<"Error: Failed to connect database." << db.lastError().text().toStdString();
        return -1;
    }
    else
    {
        LOG(INFO) << "Succeed to connect database.";
    }

    /* 创建表格 */
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec("create table TH3(time datetime primary key, temperature int, humidity int)"))
    {
        LOG(WARNING) << "Error: Fail to create table." << sqlQuery.lastError().text().toStdString();
    }
    else
    {
        LOG(INFO) << "TH3 Table Created!";
    }
    if (!sqlQuery.exec("create table TH015(time datetime primary key, pressure float, density float, temperature float)"))
    {
        LOG(WARNING) << "Error: Fail to create table015." << sqlQuery.lastError().text().toStdString();
    }
    else
    {
        LOG(INFO) << "TH15 Table Created!";
    }

    MainWindow w;

    w.show();
    return a.exec();
}
