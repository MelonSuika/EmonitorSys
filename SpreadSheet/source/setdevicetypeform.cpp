#include "setdevicetypeform.h"
#include "ui_setdevicetypeform.h"

SetDeviceTypeForm::SetDeviceTypeForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetDeviceTypeForm)
{
    ui->setupUi(this);

#if 1
    ui->tableWidget->setRowCount(20); //设置行数为20
    ui->tableWidget->setColumnCount(5);

    /* 设置列名 */
    QStringList headers;
    headers<< "表号"<<"端口号"<<"地址"<<"表类型"<<"位置";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
#endif
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox_com->addItem(info.portName());
    }


    ui->comboBox_deviceType->addItem(TYPE_NONE + "未设置类型(0)");
    ui->comboBox_deviceType->addItem("ZMJ100P(4)");
    ui->comboBox_deviceType->addItem("ZMJ60XD(4)");
    ui->comboBox_deviceType->addItem("ZMJ100PR(4)");
    ui->comboBox_deviceType->addItem("HM100PR(4)超高压015");
    ui->comboBox_deviceType->addItem("ZMJ100PRO(4)");
    ui->comboBox_deviceType->addItem("ZMJ100PRW(4)");
    ui->comboBox_deviceType->addItem("ZMJ100PRDH(4)");
    ui->comboBox_deviceType->addItem("THC(8)温湿度控制器");

    m_pComlist = nullptr;

}


SetDeviceTypeForm::~SetDeviceTypeForm()
{
    delete ui;
}

void SetDeviceTypeForm::Create(QList<SerialPortInfo> *pComlist)
{
    int index = 0;

    m_pComlist = pComlist;
    for (int i = 0; i < pComlist->size(); i++)
    {
        SerialPortInfo portInfo = pComlist->operator[](i);
        //for (int j = 0; j < portInfo.m_pDeviceList->size(); j++)
        {
            ui->tableWidget->setItem(index, 0, new QTableWidgetItem(QString::number(i+1)));
            ui->tableWidget->setItem(index, 1, new QTableWidgetItem(portInfo.m_serial->portName()));
            //ui->tableWidget->setItem(index, 2, new QTableWidgetItem(portInfo.m_pDeviceList->operator[](j).m_abyAddr.toInt()));
            ui->tableWidget->setItem(index, 3, new QTableWidgetItem(QString::number(portInfo.m_nDeviceType)));
            index++;
        }

    }

}



/*
    SetDeviceTypeForm暂时废弃

*/
void SetDeviceTypeForm::on_pushButton_clicked()
{
    for (int i = 0; i < m_pComlist->size(); i++)
    {
        SerialPortInfo *portInfo = &m_pComlist->operator[](i);
        QString strCom = ui->comboBox_com->currentText();
        if (portInfo->m_serial->portName() == strCom)
        {
            portInfo->m_nDeviceType = comtextToType(ui->comboBox_deviceType->currentText());
            qDebug()<<portInfo->m_nDeviceType;
            for (int j = 0; ; j++)
            {
                if (ui->tableWidget->item(j, 1) == nullptr)
                {
                    break;
                }
                if (strCom == ui->tableWidget->item(j, 1)->text())
                {
                    ui->tableWidget->item(j, 3);
                    ui->tableWidget->setItem(j, 3, new QTableWidgetItem(QString::number(portInfo->m_nDeviceType)));
                    break;
                }

            }
            break;
        }
    }

}
