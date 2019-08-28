#include "dashboardtabwidget.h"
#include "ui_dashboardtabwidget.h"
#include <QtQuickWidgets/QQuickWidget>
#include "rtdatasheetform.h"
#include "rtchartform.h"
#include "dashboardform.h"

DashBoardTabWidget::DashBoardTabWidget(QWidget *parent) :
    QTabWidget(parent),
    ui(new Ui::DashBoardTabWidget)
{
    ui->setupUi(this);

    /* 固定大小 */
    this->setFixedSize(900, 650);

    /* 加载qss改变界面风格 */
    QFile qssfile(":/qss/widget-black.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    this->setStyleSheet(qss);


    /* 显示表盘 */
    DashBoardForm *dash = new DashBoardForm;
    ui->verticalLayout->addWidget(dash);
    connect(this, SIGNAL(sendRtData(QJsonObject *, int)), dash, SLOT(rcvRtData(QJsonObject *, int)));

    /* 显示表格 */
    RtDataSheetForm *sheet = new RtDataSheetForm;
    ui->gridLayout_sheet->addWidget(sheet);
    connect(this, SIGNAL(sendRtData(QJsonObject *, int)), sheet, SLOT(rcvRtData(QJsonObject *, int)));


    /* 显示图表(曲线图) */
    RtChartForm *chart = new RtChartForm;
    ui->gridLayout_chart->addWidget(chart);
    connect(this, SIGNAL(sendRtData(QJsonObject *, int)), chart, SLOT(rcvRtData(QJsonObject *, int)));

}

DashBoardTabWidget::~DashBoardTabWidget()
{
    delete ui;
}

void DashBoardTabWidget::rcvRtData(QJsonObject *data, int nDeviceType)
{
    emit sendRtData(data, nDeviceType);
}

