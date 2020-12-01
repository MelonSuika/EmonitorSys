#include "addchilddeviceform.h"
#include "ui_addchilddeviceform.h"

AddChildDeviceForm::AddChildDeviceForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddChildDeviceForm)
{
    ui->setupUi(this);

    m_pComlist = nullptr;
    m_pSetting = new QSettings("./config.ini", QSettings::IniFormat);

    ui->tableWidget->setRowCount(64); //设置行数为64
    ui->tableWidget->setColumnCount(5);

    /* 设置列名 */
    QStringList headers;
    headers<< "表号"<<"端口号"<<"地址"<<"类型(协议)"<<"表盘类型";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);


    ui->horizontalLayout->setStretch(0, 6);
    ui->horizontalLayout->setStretch(1, 3);

}

AddChildDeviceForm::~AddChildDeviceForm()
{
    delete ui;
}



void AddChildDeviceForm::Create(QList<SerialPortInfo> *pComlist)
{
    if (m_pComlist != nullptr)
    {
        return;
    }
    int index = 0;

    m_pComlist = pComlist;
    for (int i = 0; i < pComlist->size(); i++)
    {
        ui->comboBox_com->addItem(pComlist->operator[](i).m_serial->portName());
    }
    if (pComlist->size() == 0)
    {
        ui->comboBox_com->addItem("无连接端口");
    }

    ui->comboBox_type->addItem(TYPE_NONE + "未设置类型(0)");
    ui->comboBox_type->addItem("45R(4)001");
    ui->comboBox_type->addItem("60R(4)048");
    ui->comboBox_type->addItem("100R(4)高压015");
    ui->comboBox_type->addItem("100R(4)普压009");
    ui->comboBox_type->addItem("100RDH(9)微水");
    ui->comboBox_type->addItem("THC(8)温湿度控制器");
    ui->comboBox_type->addItem("100PRW_1(4)无线");
    ui->comboBox_type->addItem("100PRW_2(4)无线");


    for (int i = 0; i < pComlist->size(); i++)
    {
        SerialPortInfo portInfo = pComlist->operator[](i);
        for (int j = 0; j < portInfo.m_pDeviceList->size(); j++)
        {
            ui->tableWidget->setItem(index, 0, new QTableWidgetItem(QString::number(index+1)));
            ui->tableWidget->setItem(index, 1, new QTableWidgetItem(portInfo.m_serial->portName()));
            ui->tableWidget->setItem(index, 2, new QTableWidgetItem(QString::number((uchar)portInfo.m_pDeviceList->operator[](j).m_abyAddr[1])));
            ui->tableWidget->setItem(index, 3, new QTableWidgetItem(QString::number(portInfo.m_pDeviceList->operator[](j).m_nDeviceType)));
            ui->tableWidget->setItem(index, 4, new QTableWidgetItem(QString::number(portInfo.m_pDeviceList->operator[](j).m_nDMJType)));
            index++;
        }
    }
}

/*
    函数功能:
    添加或者修改设备信息
*/
void AddChildDeviceForm::on_pushButton_add_clicked()
{
    if (m_pComlist->size() == 0)
    {
        QMessageBox::warning(NULL, QStringLiteral("警告"), QStringLiteral("当前无连接端口"), QMessageBox::Ok);
        return ;
    }
    for (int i = 0; i < m_pComlist->size(); i++)
    {
        SerialPortInfo *portInfo = &m_pComlist->operator[](i);
        bool flag = false;
        /* 地址和设备类型决定设备 */
        QString str = ui->lineEdit_addr->text();
        int nDType = comtextToType(ui->comboBox_type->currentText());
        /* 未选类型，警告并返回 */
        if (nDType == TYPE_NONE)
        {
            QMessageBox::warning(NULL, QStringLiteral("警告"), QStringLiteral("请选择设备类型"), QMessageBox::Ok);
            return;
        }
        /* 设备类型 */
        int nDMJType = comtext2DMJType(ui->comboBox_type->currentText());
        qDebug()<<"-------------nDMJType"<<nDMJType;

        /* 先查找串口号是否一致，
            再查找地址是否一致
        */
        if (ui->comboBox_com->currentText() == portInfo->m_serial->portName())
        {
            for (int j = 0; j < portInfo->m_pDeviceList->size(); j++)
            {
                DeviceInfo info = portInfo->m_pDeviceList->operator[](j);
                if (QString::number(info.m_abyAddr[1]).toInt() == str.toInt())
                {
                    flag = true;
                    break;
                }

            }
        }
        else
        {
            /* 串口号不一致，直接查找下一个串口 */
            continue;
        }
        if (flag == false)
        {
            portInfo->m_nDeviceType = nDType;
            //portInfo->m_nDMJType = nDMJType;

            DeviceInfo info;
            int n = str.toInt();
            info.m_abyAddr[0] = 0;
            info.m_abyAddr[1] = n;
            info.m_nDeviceType = nDType;
            info.m_nDMJType = nDMJType;


            qDebug()<<"新增地址为:"<<QString::number(info.m_abyAddr[0], 16)<<QString::number((uchar)info.m_abyAddr[1], 16);
            LOG(INFO)<<QString("新增地址为:%1").arg((uchar)info.m_abyAddr[1]).toStdString();
            m_pComlist->operator[](i).m_pDeviceList->append(info);
            m_pSetting->setValue("address/"+QString::number((uchar)info.m_abyAddr[1]), QString("%1,%2").arg(nDType).arg(nDMJType));
            for (int k = 0; ; k++)
            {
                if(ui->tableWidget->item(k, 0) == nullptr)
                {
                    ui->tableWidget->setItem(k, 0, new QTableWidgetItem(QString::number(k+1)));
                    ui->tableWidget->setItem(k, 1, new QTableWidgetItem(portInfo->m_serial->portName()));
                    ui->tableWidget->setItem(k, 2, new QTableWidgetItem(QString::number((uchar)info.m_abyAddr[1])));
                    ui->tableWidget->setItem(k, 3, new QTableWidgetItem(QString::number(nDType)));
                    ui->tableWidget->setItem(k, 4, new QTableWidgetItem(QString::number(nDMJType)));
                    break;
                }
            }
        }
        else
        {
            if (portInfo->m_nDeviceType != nDType)
            {
                /* 设备类型 */
                portInfo->m_nDeviceType = nDType;
                //portInfo->m_nDMJType = nDMJType;
                for (int k = 0; ; k++)
                {
                    if(ui->tableWidget->item(k, 0) == nullptr)
                    {
                        break;

                    }

                    ui->tableWidget->setItem(k, 3, new QTableWidgetItem(QString::number(portInfo->m_nDeviceType)));
                }
            }

        }

    }
}


/*
    函数功能:
    加载配置文件
*/
void AddChildDeviceForm::on_pushButton_onloadIni_clicked()
{
    if (m_pComlist->size() == 0)
    {
        return;
    }

    int index = 0;
    m_pSetting->beginGroup("address");
    int size = m_pSetting->allKeys().size();

    int nComIndex = 0;
    /* 匹配串口 */
    for (int i = 0; i < m_pComlist->size(); i++)
    {
        if (m_pComlist->operator[](i).m_serial->portName() == ui->comboBox_com->currentText())
        {
            nComIndex = i;
            break;
        }

    }
    for(int i = 0; i < size; i++)
    {
        QString strAddress = m_pSetting->allKeys().at(i);
        //QString strDeviceType = m_pSetting->value(strAddress).toString();
        QString strType = m_pSetting->value(strAddress).toString();
        QString strProtocol = strType.section(',', 0, 0).trimmed();
        QString strDMJType = strType.section(',', 1, 1);
        ui->tableWidget->setItem(index, 0, new QTableWidgetItem(QString::number(index+1)));
        ui->tableWidget->setItem(index, 1, new QTableWidgetItem(ui->comboBox_com->currentText()));
        ui->tableWidget->setItem(index, 2, new QTableWidgetItem(strAddress));
        ui->tableWidget->setItem(index, 3, new QTableWidgetItem(strProtocol));
        ui->tableWidget->setItem(index, 4, new QTableWidgetItem(strDMJType));;
        DeviceInfo info;
        int n = strAddress.toInt();
        info.m_abyAddr[0] = 0;
        info.m_abyAddr[1] = n;
        info.m_nDeviceType = strProtocol.toInt();
        info.m_nDMJType = strDMJType.toInt();
        m_pComlist->operator[](nComIndex).m_pDeviceList->append(info);
        index++;
    }
    m_pSetting->endArray();
}
