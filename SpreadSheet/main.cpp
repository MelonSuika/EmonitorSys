#include "mainwindow.h"
#include <QApplication>
#include "connection.h"
#include <QProcess>
#include <QStyleFactory>
#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QTime>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* 建立并打开数据库 */
    QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
    db.setDatabaseName( "test.db" );

    if (!db.open())
    {
        qDebug() << "Error: Failed to connect database." << db.lastError();
        return -1;
    }
    else
    {
        qDebug() << "Succeed to connect database.";
    }

    /* 创建表格 */
    QSqlQuery sqlQuery;
    if(!sqlQuery.exec("create table TH3(time datetime primary key, temperature int, humidity int)"))
    {
        qDebug() << "Error: Fail to create table."<< sqlQuery.lastError();
    }
    else
    {
        qDebug() << "Table created!";
    }

    /* 插入数据 */
    /*QTime::currentTime().toString()*/
    /*if(!sqlQuery.exec("INSERT INTO THC VALUES('8:55:41', 25, 25)"))
    {
        qDebug() << sqlQuery.lastError();
    }
    else
    {
        qDebug() << "inserted value 1,25,25!";
    }*/


    MainWindow w;

    w.show();

    return a.exec();
}
