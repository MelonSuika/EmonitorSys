#include "dashboardtabwidget.h"
#include "ui_dashboardtabwidget.h"
#include <QtQuickWidgets/QQuickWidget>


DashBoardTabWidget::DashBoardTabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::DashBoardTabWidget)
{
    ui->setupUi(this);

    /* 显示表盘 */
    DashBoardForm *dash = new DashBoardForm;
    ui->verticalLayout->addWidget(dash);
    connect(this, SIGNAL(sendRtData(QJsonObject *, DeviceSymbolInfo)), dash, SLOT(rcvRtData(QJsonObject *, DeviceSymbolInfo)));

    /* 监测表界面 */
    RtMonitorForm *monitor = new RtMonitorForm;
    ui->gridLayout_monitor->addWidget(monitor);
    connect(this, SIGNAL(sendRtData(QJsonObject *, DeviceSymbolInfo)), monitor, SLOT(rcvRtData(QJsonObject *, DeviceSymbolInfo)));

    /* 显示表格 */
    RtDataSheetForm *sheet = new RtDataSheetForm;
    ui->gridLayout_sheet->addWidget(sheet);
    connect(this, SIGNAL(sendRtData(QJsonObject *, DeviceSymbolInfo)), sheet, SLOT(rcvRtData(QJsonObject *, DeviceSymbolInfo)));


    /* 显示图表(曲线图) */
    RtChartForm *chart = new RtChartForm;
    ui->gridLayout_chart->addWidget(chart);
    connect(this, SIGNAL(sendRtData(QJsonObject *, DeviceSymbolInfo)), chart, SLOT(rcvRtData(QJsonObject *, DeviceSymbolInfo)));

}

DashBoardTabWidget::~DashBoardTabWidget()
{
    delete ui;
}

void DashBoardTabWidget::rcvRtData(QJsonObject *data, DeviceSymbolInfo deviceSbInfo)
{
    /*
        Note:这边最好将单设备和多设备的界面信号分开来处理
    */
    int t = data->value("温度").toInt();
    int p = data->value("压力").toInt();
    int c = data->value("密度").toInt();
    qDebug()<<"first data:t, p, c, address, DMJType"<<t<<p<<c<<deviceSbInfo.nAddress<<deviceSbInfo.nDMJType;
    emit sendRtData(data, deviceSbInfo);

}

