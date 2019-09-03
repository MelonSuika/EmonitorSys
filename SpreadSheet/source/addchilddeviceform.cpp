#include "addchilddeviceform.h"
#include "ui_addchilddeviceform.h"

AddChildDeviceForm::AddChildDeviceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddChildDeviceForm)
{
    ui->setupUi(this);

    /* 加载qss改变界面风格 */
    QFile qssfile(":/qss/widget-blue.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    this->setStyleSheet(qss);


    ui->tableWidget->setRowCount(30); //设置行数为20
    ui->tableWidget->setColumnCount(5);

    /* 设置列名 */
    QStringList headers;
    headers<< "表号"<<"端口号"<<"地址"<<"表类型"<<"位置";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->horizontalLayout->setStretch(0, 5);
    ui->horizontalLayout->setStretch(1, 2);
}

AddChildDeviceForm::~AddChildDeviceForm()
{
    delete ui;
}


void AddChildDeviceForm::Create(QList<SerialPortInfo> *pComlist)
{
    int index = 0;

    m_pComlist = pComlist;
    for (int i = 0; i < pComlist->size(); i++)
    {
        ui->comboBox_com->addItem(pComlist->operator[](i).m_serial->portName());
    }

    ui->comboBox_type->addItem(TYPE_NONE + "未设置类型(0)");
    ui->comboBox_type->addItem("ZMJ100P(4)");
    ui->comboBox_type->addItem("ZMJ60XD(4)");
    ui->comboBox_type->addItem("ZMJ100PR(4)");
    ui->comboBox_type->addItem("HM100PR(4)超高压015");
    ui->comboBox_type->addItem("ZMJ100PRO(4)");
    ui->comboBox_type->addItem("ZMJ100PRW(4)");
    ui->comboBox_type->addItem("ZMJ100PRDH(4)");
    ui->comboBox_type->addItem("THC(8)温湿度控制器");

    for (int i = 0; i < pComlist->size(); i++)
    {
        SerialPortInfo portInfo = pComlist->operator[](i);
        for (int j = 0; j < portInfo.m_pDeviceList->size(); j++)
        {
            ui->tableWidget->setItem(index, 0, new QTableWidgetItem(QString::number(i+1)));
            ui->tableWidget->setItem(index, 1, new QTableWidgetItem(portInfo.m_serial->portName()));
            ui->tableWidget->setItem(index, 2, new QTableWidgetItem(QString::number(portInfo.m_pDeviceList->operator[](j).m_abyAddr[1])));
            ui->tableWidget->setItem(index, 3, new QTableWidgetItem(QString::number(portInfo.m_nDeviceType)));
            index++;
        }

    }
}

void AddChildDeviceForm::on_pushButton_ok_clicked()
{
    for (int i = 0; i < m_pComlist->size(); i++)
    {
        SerialPortInfo portInfo = m_pComlist->operator[](i);
        bool flag = false;
        QString str = ui->lineEdit_addr->text();
        if (ui->comboBox_com->currentText() == portInfo.m_serial->portName())
        {
            for (int j = 0; j < portInfo.m_pDeviceList->size(); j++)
            {
                DeviceInfo info = portInfo.m_pDeviceList->operator[](j);
                if (QString::number(info.m_abyAddr[1]).toInt() == str.toInt())
                {
                    flag = true;
                    break;
                }
            }
        }
        if (flag == false)
        {
            DeviceInfo info;
            int n = str.toInt();
            info.m_abyAddr[0] = 0;
            qDebug()<<QString::number(info.m_abyAddr[0], 16);
            info.m_abyAddr[1] = n;
            qDebug()<<QString::number((uchar)info.m_abyAddr[1], 16);
            m_pComlist->operator[](i).m_pDeviceList->append(info);
            for (int k = 0; ; k++)
            {
                if(ui->tableWidget->item(k, 0) == nullptr)
                {
                    ui->tableWidget->setItem(k, 0, new QTableWidgetItem(QString::number(k+1)));
                    ui->tableWidget->setItem(k, 1, new QTableWidgetItem(portInfo.m_serial->portName()));
                    ui->tableWidget->setItem(k, 2, new QTableWidgetItem(QString::number((uchar)info.m_abyAddr[1])));
                    ui->tableWidget->setItem(k, 3, new QTableWidgetItem(QString::number(portInfo.m_nDeviceType)));
                    break;
                }
            }
        }

    }
#if 0
    for (int i = 0; i < m_pComlist->size(); i++)
    {
        qDebug()<<"size = "<<m_pComlist->operator[](i).m_pDeviceList->size();
        for (int j = 0; j < m_pComlist->operator[](i).m_pDeviceList->size(); j++)
        {
            qDebug()<<"addr:"<<m_pComlist->operator[](i).m_pDeviceList->operator[](j).m_abyAddr.toHex();

        }

    }
#endif

}
