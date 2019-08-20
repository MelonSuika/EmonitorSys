#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "spreadsheet.h"
#include <QSerialPort>
#include "console.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include "common.h"
#include "serialportinfo.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendRtData(int data);

private slots:
    void errorFunc(QSerialPort::SerialPortError err);

    void on_pushButtonSheet_clicked();

    void on_pushButtonSerial_clicked();

    void on_pushButtonWriteSerialPort_clicked();

    void readData();

    void on_pushButtonReadData_clicked();

    void SendMsgFunc();

    void on_pushButtonGageStatus_clicked();

    void on_pushButtonConnectInfo_clicked();

    void on_pushButton_clicked();

    void on_pushButtonChartStatus_clicked();

    void on_pushButton_dashBoard_clicked();

    void on_pushButtonAllSheet_clicked();

private:
    Ui::MainWindow *ui;
    SpreadSheet *m_pSpreadSheet;
    Console *m_console = nullptr;
    /* 串口数组 */
    QTimer *m_timer;
    bool isRunFlag;
    QByteArray m_abyBuffer;     /* 接收数据备份缓存 */
    int m_nWaitSerialCnt;       /* 缓存区数据不够计数 */
    bool m_isChkAdrCmd;         /* 查询地址命令 */
    QSqlQuery m_sqlQuery;

    QSqlQueryModel *m_model;
    QTableView *m_view;

    /* 表盘tab窗口界面 */
    DashBoardTabWidget *m_dBwdgt;


    QList<SerialPortInfo> *m_pComlist;
    int m_nComCount;


};

#endif // MAINWINDOW_H
