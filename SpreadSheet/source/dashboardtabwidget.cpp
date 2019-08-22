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
    connect(this, SIGNAL(sendRtData(int)), dash, SLOT(rcvRtData(int)));

    /* 显示表格 */
    RtDataSheetForm *sheet = new RtDataSheetForm;
    ui->gridLayout_sheet->addWidget(sheet);

    /* 显示图表(曲线图) */
    RtChartForm *chart = new RtChartForm;
    ui->gridLayout_chart->addWidget(chart);


}

DashBoardTabWidget::~DashBoardTabWidget()
{
    delete ui;
}

void DashBoardTabWidget::rcvRtData(int n)
{
    qDebug()<<"DashBoardTabWidget's rcvRtData = "<<n;
    emit sendRtData(n);
}

