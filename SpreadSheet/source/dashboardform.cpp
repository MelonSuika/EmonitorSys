#include "dashboardform.h"
#include "ui_dashboardform.h"
#include <QtQuickWidgets/QQuickWidget>
#include <QSerialPortInfo>
#include <QQmlEngine>
#include <QQmlContext>
#include <QJsonObject>

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


    m_quickWidget = new QQuickWidget;
    m_quickWidget->setSource(QUrl("qrc:/dialcontrol.qml"));
    ui->verticalLayout_dashBoard->addWidget(m_quickWidget);


    /* 数据交互  */
    m_person = 80;
    m_quickWidget->engine()->rootContext()->setContextProperty("person", m_person);

    ui->gridLayout->setColumnStretch(0, 7);
    ui->gridLayout->setColumnStretch(1, 3);

}

DashBoardForm::~DashBoardForm()
{
    delete ui;
}

void DashBoardForm::on_pushButton_clicked()
{
    //m_person = rand()%200;
    //m_quickWidget->engine()->rootContext()->setContextProperty("person", m_person);
    //qDebug()<<"person = "<<m_person;

}

void DashBoardForm::rcvRtData(QJsonObject *data, int nDeviceType)
{
    m_person = data->value("温度").toInt();
    m_quickWidget->engine()->rootContext()->setContextProperty("person", m_person-2900);
    ui->lineEdit_temperature->setText(QString::number(data->value("温度").toInt()));
    ui->lineEdit_pressure->setText(QString::number(data->value("压力").toInt()));
    ui->lineEdit_density->setText(QString::number(data->value("密度").toInt()));


}
