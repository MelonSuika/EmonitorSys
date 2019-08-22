#include "setdevicetypeform.h"
#include "ui_setdevicetypeform.h"

SetDeviceTypeForm::SetDeviceTypeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetDeviceTypeForm)
{
    ui->setupUi(this);



#if 1
    ui->tableWidget->setRowCount(20); //设置行数为10
    ui->tableWidget->setColumnCount(4);

    /* 设置列名 */
    QStringList headers;
    headers<< "表号"<<"端口号"<<"表类型"<<"位置";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
#endif
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

}


SetDeviceTypeForm::~SetDeviceTypeForm()
{
    delete ui;
}

void SetDeviceTypeForm::Create(QList<SerialPortInfo> &pComlist)
{
#if 0
    for (int i = 0; i < 5; i++)
    {
        ui->tableWidget->setItem(1, 1, new QTableWidgetItem("aaa"));



    }
#endif
    for (int i = 0; i < pComlist.size(); i++)
    {



    }

}
