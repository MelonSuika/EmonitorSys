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
#if 0
    QQuickWidget *quickWidget = new QQuickWidget;
    quickWidget->setSource(QUrl("qrc:/dialcontrol.qml"));
    ui->verticalLayout->addWidget(quickWidget);
#endif


    /* 显示表格 */
#if 1
    RtDataSheetForm *sheet = new RtDataSheetForm;
    ui->gridLayout_sheet->addWidget(sheet);
#endif

    /* 显示图表(曲线图) */
#if 1
    RtChartForm *chart = new RtChartForm;
    ui->gridLayout_chart->addWidget(chart);
#endif


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

