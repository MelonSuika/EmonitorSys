#include "rtdatasheetform.h"
#include "ui_rtdatasheetform.h"

RtDataSheetForm::RtDataSheetForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RtDataSheetForm)
{
    ui->setupUi(this);

    //ui->tableWidget;

    ui->tableWidget->setRowCount(65); //设置行数为10
    ui->tableWidget->setColumnCount(5);

    /* 设置列名 */
    QStringList headers;
    headers<< "表号"<<"温度(℃)"<<"压力(MPa)"<<"密度(MPa)"<<"位置信息";
    ui->tableWidget->setHorizontalHeaderLabels(headers);


}

RtDataSheetForm::~RtDataSheetForm()
{
    delete ui;
}

void RtDataSheetForm::rcvRtData(QJsonObject *data, DeviceSymbolInfo deviceSbInfo)
{
    qDebug()<<"33333"<<deviceSbInfo.nDMJType;
    if (deviceSbInfo.nDeviceIndex > 0)
        return;

}
