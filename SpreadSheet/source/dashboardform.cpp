#include "dashboardform.h"
#include "ui_dashboardform.h"
#include <QtQuickWidgets/QQuickWidget>
#include <QSerialPortInfo>
#include <QQmlEngine>
#include <QQmlContext>
#include <QJsonObject>
#include "rtchartform.h"


DashBoardForm::DashBoardForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DashBoardForm)
{
    ui->setupUi(this);


    /* 下拉框初始 */
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox_com->addItem(info.portName());
    }


    /* 加载qss改变界面风格 */
    QFile qssfile(":/qss/widget-black.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    this->setStyleSheet(qss);


    /* 加载表盘 */
    m_quickWidget = new QQuickWidget;
    m_quickWidget->setSource(QUrl("qrc:/dialcontrol.qml"));
    ui->verticalLayout_dashBoard->addWidget(m_quickWidget);
    /* 固定大小 */
    //this->setFixedSize(900, 650);
    m_quickWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    m_quickWidget->setFixedSize(625, 600);

    /* chart */
    m_chartForm = new RtChartForm;
    ui->gridLayout_chart->addWidget(m_chartForm);
    connect(this, SIGNAL(sendToMChart(QJsonObject *, int)), m_chartForm, SLOT(rcvRtData(QJsonObject *, int)));

    ui->pushButton_rtDisplay->hide();

    /* 数据交互  */
    m_nPressure = 0;
    m_quickWidget->engine()->rootContext()->setContextProperty("pressure", m_nPressure);

    ui->gridLayout->setColumnStretch(0, 7);
    ui->gridLayout->setColumnStretch(1, 3);

}

DashBoardForm::~DashBoardForm()
{
    delete ui;
}


void DashBoardForm::rcvRtData(QJsonObject *data, int nDeviceType)
{
    QString str = ui->lineEdit_address->text();
    if (str == "" || str == QString::number(data->value("地址").toInt()))
    {

        emit sendToMChart(data, nDeviceType);
        int t = data->value("温度").toInt();
        int p = data->value("压力").toInt();
        int c = data->value("密度").toInt();
        m_nPressure = p;

        m_quickWidget->engine()->rootContext()->setContextProperty("pressure", (float)(m_nPressure+320)/12);

        ui->lineEdit_address->setText(QString::number(data->value("地址").toInt()));
        ui->lineEdit_temperature->setText(QString::number((float)t/100));
        ui->lineEdit_pressure->setText(QString::number((float)p/10000));
        ui->lineEdit_density->setText(QString::number((float)c/10000));
    }


}
