#ifndef COMMON_H
#define COMMON_H

#define COM_COUNT_MAX 128

#include <QSqlError>
#include <QSqlQuery>
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


#include "dashboardform.h"
#include "dashboardtabwidget.h"

#include "rtdatasheetform.h"
#include "rtchartform.h"


#define TH_CHK_ADDR(addr, count) {addr, 0x03, 0xff, 0x00, 0x00, count}
#define TH_CHK_DATA(addr, count) {addr, 0x03, 0x00, 0x00, 0x00, count}


enum DeviceType{

    TYPE_NONE = 0,
    ZMJ100P = 1,
    ZMJ60XD,
    ZMJ100PR,
    HM100PR,
    ZMJ100PRO,
    ZMJ100PRW,
    ZMJ100PRDH,
    THC
};

#endif // COMMON_H
