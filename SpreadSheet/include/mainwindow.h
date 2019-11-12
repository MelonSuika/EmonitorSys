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
#include "datasheetform.h"
#include "chartform.h"
#include "setdevicetypeform.h"
#include "addchilddeviceform.h"
#include "gasrelaymonitorform.h"
#include "myqquickwidget.h"


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
    void sendRtData(QJsonObject *data, int deviceType);
    void sigSendMsg(int nIndex);
    void sigDelayRead(int nIndex);

private slots:
    void errorFunc(QSerialPort::SerialPortError err);

    void on_pushButtonSheet_clicked();

    void on_pushButtonSerial_clicked();

    void on_pushButtonWriteSerialPort_clicked();

    void readData();

    void on_pushButtonReadData_clicked();

    void SendMsgFunc(int nIndex = 0);

    void on_pushButtonConnectInfo_clicked();

    void on_pushButtonChartStatus_clicked();

    void on_pushButton_dashBoard_clicked();

    void on_pushButtonAllSheet_clicked();

    void on_pushButton_setDeviceType_clicked();

    void comDelay();

    void on_pushButton_addChildDevice_clicked();

    void on_pushButton_readSet_clicked();

    void on_pushButton_clicked();

    void setQuickWidgetContextProperty();

    void readDeviceData(DeviceSymbolInfo deviceSbInfo);



private:
    Ui::MainWindow *ui;
    SpreadSheet *m_pSpreadSheet;
    Console *m_console = nullptr;
    /* 串口数组 */
    QTimer *m_timer;            /* 定时读取 */
    QTimer *m_delayTimer;       /* 延时读取timer */
    QTimer *m_gasTimer;         /* gas timer */
    //QTimer *m_delayReadTimer;   /* 延时读取 */
    bool isRunFlag;
    QByteArray m_abyBuffer;     /* 接收数据备份缓存 */
    int m_nWaitSerialCnt;       /* 缓存区数据不够计数 */
    bool m_isChkAdrCmd;         /* 查询地址命令 */
    QSqlQuery m_sqlQuery;

    QSqlQueryModel *m_model;
    QTableView *m_view;

    /* 表盘tab窗口界面 */
    QPointer<DashBoardTabWidget> m_dBwdgt;
    /* 数据表界面 */
    QPointer<DataSheetForm> m_sheetForm;
    /* 曲线图界面 */
    QPointer<ChartForm> m_chartForm;
    /* 设置界面 */
    QPointer<SetDeviceTypeForm> m_setForm;
    /* 添加子设备界面 */
    QPointer<AddChildDeviceForm> m_addForm;
    /* 瓦斯继电器监测界面 */
    QPointer<GasRelayMonitorForm> m_gasForm;
    /* 瓦斯继电器监测界面 */
    QPointer<MyQQuickWidget> m_gasQuickForm;

    QList<SerialPortInfo> *m_pComlist;
    int m_nComCount;
    int m_nCount;
    QByteArray m_adrBuffer;

    int m_nReadTimeGap;


    int m_test;

    bool m_isReadSuccess;


};

#endif // MAINWINDOW_H
