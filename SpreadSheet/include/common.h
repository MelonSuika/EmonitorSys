#ifndef COMMON_H
#define COMMON_H

#include <algorithm>

#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlQueryModel>

#include <QTableView>
#include <QHeaderView>

#include <QSerialPortInfo>
#include <QSerialPort>

#include <QList>
#include <QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

#include <QVector>
#include <QPointer>

#include <QJsonObject>
#include <QQuickWidget>
#include <QTime>

#include <QThread>
#include <QSettings>

#include <QPrinter>

/* windows下宏冲突，添加该宏防止编译报错 */
#define GLOG_NO_ABBREVIATED_SEVERITIES
#include "glog/config.h"
#include "glog/logging.h"


/* 温湿度传感器 */
#define TH_CHK_ADDR(addr, count) {addr, 0x03, 0xff, 0x00, 0x00, count}
#define TH_CHK_DATA(addr, count) {addr, 0x03, 0x00, 0x00, 0x00, count}

/* HM100PR超高压远传015 */
#define HM_CHK_ADDR(addr, count) {0x00, 0x04, 0xff, 0x00, 0x00, count}
#define HM_CHK_DATA(addr, count) {addr, 0x04, 0x00, 0x00, 0x00, count}

/* RDH */
#define RDH_CHK_DATA(addr, count) {addr, 0x04, 0x00, 0x00, 0x00, count}
/* 设备信息，设备是由com口和设备号来标识的 */
typedef struct deviceSymbolInfo
{
    int nComIndex;
    int nDeviceIndex;
    int nAddress;
    int nDeviceType;
    int nDMJType;
}DeviceSymbolInfo;

enum DeviceType{

    TYPE_NONE = 0,
    //ZMJ100P = 1,
    //ZMJ60XD,
    ZMJ100PR,
    HM100PR = 4,
    ZMJ100PRO,
    ZMJ100PRW,
    ZMJ100PRDH,
    THC,
    RDH
};

/* 表盘类型 */
enum DMJType{
    DMJ_NONE = 0,
    DMJ100PR_1 = 10,
    DMJHM100PR_1 = 20,
    DMJ60PR_1 = 30,
    DMJ100PRW_1 = 40,
    DMJ100PRW_2,
    DT_1  = 50,
    DMJ45PR_1 = 60,
    DMJ_RDH_1 = 70,
};

#define COM_COUNT_MAX 128

/* 轮询间隔 */
#define POLL_GAP 500

/* 监测界面每页最大的设备数 */
#define PAGE_MAXCOUNT 30
/* 每页最大行数 */
#define PAGE_MAXROW    6
/* 每页最大列数 */
#define PAGE_MAXCOLUMN 5

/* 皮肤路径 */
#define SKIN_BLUE ":/qss/widget-blue.qss"
#define SKIN_DARK ":/qss/widget-black.qss"
#define SKIN_ORANGE ":/qss/widget-darkOrange.qss"

/* ep material前缀 */
#define PRE_FILEHREADER "./"

int comtextToType(QString str);
int comtext2DMJType(QString str);
int Arg2DMJType(QString arg);

/* log文件 */
#define LOGFILE "run.log"

/* 数据库名 */
#define DATABASENAME "MTBF2.db"


#endif // COMMON_H
