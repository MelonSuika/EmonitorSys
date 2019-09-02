#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLayout"
#include "spreadsheet.h"
#include <QThread>
#include <QtConcurrent>
#include <windows.h>
#include <QTimer>
#include <QSerialPortInfo>
#include <QtQuickWidgets/QQuickWidget>
#include "chartform.h"
#include "datasheetform.h"
#include "setdevicetypeform.h"
#include "addchilddeviceform.h"
#include <memory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pComlist(new QList<SerialPortInfo>())
{
    ui->setupUi(this);    

    isRunFlag           = false;
    m_timer             = new QTimer(this);
    m_delayTimer        = new QTimer(this);

    /* 加载qss改变界面风格 */
    QFile qssfile(":/qss/widget-blue.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    this->setStyleSheet(qss);

    /* textEditDebug防止信息太多崩溃 */
    ui->textEditDebug->document()->setMaximumBlockCount(100);


    /* COM监测相关初始化 */
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug()<<"Name:"<<info.portName();
        qDebug()<<"Description:"<<info.description();
        qDebug()<<"Manufacturer:"<<info.manufacturer();

        SerialPortInfo serialPortInfo;

        //这里相当于自动识别串口号之后添加到了cmb，如果要手动选择可以用下面列表的方式添加进去
        serialPortInfo.m_serial->setPort(info);
        if(serialPortInfo.m_serial->open(QIODevice::ReadWrite))
        {
            m_pComlist->append(serialPortInfo);
            //将串口号添加到cmb
            ui->comboBoxIsActiveCom->addItem(info.portName());
            //关闭串口等待人为(打开串口按钮)打开
            serialPortInfo.m_serial->close();
            connect(serialPortInfo.m_serial, SIGNAL(readyRead()), this, SLOT(comDelay()));

            m_nComCount++;
        }
    }
    connect(m_delayTimer, SIGNAL(timeout()), this, SLOT(readData()));

    /* 初始化指针变量 */
    m_view = nullptr;
    m_model = nullptr;

    m_dBwdgt = nullptr;
    m_sheetForm = nullptr;
    m_chartForm = nullptr;
    m_setForm = nullptr;
    m_addForm = nullptr;
    m_nCount = 0;
}


MainWindow::~MainWindow()
{
    delete ui;
    delete m_timer;
}

/*
    函数功能:掉线提示
*/
void MainWindow::errorFunc(QSerialPort::SerialPortError err)
{
    if (err == QSerialPort::ResourceError)
    {
        qDebug()<<"串口拔出";
    }
}


/*
    函数功能:数据表显示
*/
void MainWindow::on_pushButtonSheet_clicked()
{
    m_pSpreadSheet = new SpreadSheet(13, 6);
    m_pSpreadSheet->show();
    m_pSpreadSheet->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

/*
    函数功能:串口通信测试
*/
void MainWindow::on_pushButtonSerial_clicked()
{

    int nCntCom = 0;
    for (int i = 0; i < m_pComlist->size(); i++)
    {
        QSerialPort *serial = m_pComlist->at(i).m_serial;
        QSerialPortInfo *serialInfo = m_pComlist->at(i).m_serialInfo;
        if (false == serial->isOpen())
        {
            serial->setPortName(serial->portName());
            serial->setBaudRate(9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            if (serial ->open(QIODevice::ReadWrite))
            {
                ui->textEditTest->append("Open serial " + serial->portName() + " success");
                ui->pushButtonSerial->setText("关闭串口");
            }
            else
            {
                ui->textEditTest->append("Open serial " + serial->portName() + " fail");
            }
        }
        else
        {
            serial->close();
            ui->textEditTest->append("close serial " + serial->portName() + " success");
            ui->pushButtonSerial->setText("打开串口");
            /* 停止定时器 */
            m_timer->stop();
        }
        nCntCom++;

    }
    if (0 == nCntCom)
    {
        ui->textEditTest->append("无串口连接");
    }

}

/*
    函数功能:CRC生成
*/
unsigned short Get_CRC(uchar *pBuf, int len)
{
    int i, j;
    unsigned short wCrc = 0xFFFF;
    for (i = 0; i < len; i++)
    {
        wCrc ^= (unsigned short)(pBuf[i]);
        for (j = 0; j < 8; j++)
        {
            if(wCrc&1)
            {
                wCrc >>= 1;
                wCrc ^= 0xA001;
            }
            else
            {
                wCrc >>= 1;
            }
        }
    }
    return wCrc;
}

/*
    函数功能:串口写数据，查询传感器地址
    note:COM查询地址的时候是广播形式的，所以只需要遍历端口
*/
void MainWindow::on_pushButtonWriteSerialPort_clicked()
{
    for (int i = 0; i < m_pComlist->size(); i++)
    {       
        QSerialPort *serial = m_pComlist->at(i).m_serial;
        SerialPortInfo portInfo = m_pComlist->at(i);
        m_pComlist->operator[](i).m_isChkAdrCmd = true;

        if (portInfo.m_nDeviceType == TYPE_NONE)
        {
            ui->textEditTest->append(portInfo.m_serial->portName() + "未设置表类型");
            continue;
        }
        if (serial->isOpen())
        {
            unsigned short wCrc = 0;
            QByteArray abyd;
            unsigned char d[8];

            switch (portInfo.m_nDeviceType)
            {
                case THC:
                {
                    unsigned char td[8] = TH_CHK_ADDR(0x00, 0x01);
                    memcpy(d, td, 6);
                    break;
                }
                case HM100PR:
                {
                    unsigned char td[8] = HM_CHK_ADDR(0x00, 0x01);
                    memcpy(d, td, 6);
                    break;
                }
                case TYPE_NONE:
                    break;
            }

            wCrc = Get_CRC(d, 6);
            d[7] = (wCrc&0xff00)>>8;
            d[6] = (wCrc&0x00ff);
            abyd.resize(8);
            for (int i = 0; i < 8; i++)
            {
                abyd[i] = d[i];
            }
            if (serial->isOpen())
            {
                serial->write(abyd);
            }
            ui->textEditTest->append(serial->portName() + " send to lower" + " " + abyd.toHex());
        }
        else
        {
            ui->textEditTest->setText("串口 " + serial->portName() + " 未打开");
        }
    }
    if (m_pComlist->size() == 0)
    {
        ui->textEditTest->append("无串口连接");
    }
}

/*
 *  THC表
    函数功能:温湿度解析
*/
void analysisTH(QByteArray dataIn, int &tOut, int &hOut)
{
    tOut = ((uchar)dataIn[3]*256) + ((uchar)dataIn[4]);
    hOut = ((uchar)dataIn[5]*256) + ((uchar)dataIn[6]);
}

/*
    函数功能:HM100PR解析
*/
void analysisHM100PR(QByteArray dataIn, int &tOut, int &pOut, int &cOut)
{
    tOut = ((uchar)dataIn[3]*256) + ((uchar)dataIn[4]);
    pOut = ((uchar)dataIn[5]*256) + (uchar)dataIn[6];
    cOut = ((uchar)dataIn[7]*256) + (uchar)dataIn[8];
}

/*
    函数功能:延时读取
*/
void MainWindow::comDelay()
{
    qDebug()<<"comDelay";
    m_delayTimer->start(100);

}

/*
    函数功能:串口读数据
*/
void MainWindow::readData()
{

    m_delayTimer->stop();
    for (int i = 0; i < m_pComlist->size(); i++)
    {
        int tOut = -1, hOut = -1, pOut = -1, cOut = -1;
        QSerialPort *serial = m_pComlist->at(i).m_serial;
        SerialPortInfo *serialPortInfo = &m_pComlist->operator[](i);
        qDebug()<<"bytes:"<<serial->bytesAvailable();
        if (serialPortInfo->m_nDeviceType == THC)
        {
            /* 如果是第一次查询地址后的第一次读取，需要判断缓存是否等于7 */
            if (serialPortInfo->m_isChkAdrCmd == true && serial->bytesAvailable() < 7)
            {
                serialPortInfo->m_nWaitSerialCnt++;
                continue;
            }
            /* 如果不是查询后的第一次读取，那就是温湿度读取，判断是否大于9 */
            if (serialPortInfo->m_isChkAdrCmd == false && serial->bytesAvailable() != 9)
            {
                serialPortInfo->m_nWaitSerialCnt++;
                continue;
            }
        }
        else if(serialPortInfo->m_nDeviceType == HM100PR)
        {
            /* 如果是第一次查询地址后的第一次读取，需要判断缓存是否等于7 */
            if (serialPortInfo->m_isChkAdrCmd == true && serial->bytesAvailable() < 7)
            {
                serialPortInfo->m_nWaitSerialCnt++;
                continue;
            }
            /* 如果不是查询后的第一次读取，那就是温湿度读取，判断是否大于11 */
            if (serialPortInfo->m_isChkAdrCmd == false && serial->bytesAvailable() < 11)
            {
                serialPortInfo->m_nWaitSerialCnt++;
                continue;
            }
        }


        QJsonObject *obj = new QJsonObject;
        if (serialPortInfo->m_isChkAdrCmd == false)
        {

            /*
                数据库中以表为组织单位存储数据
                表类似我们的类，每个字段都有对应的数据类型。
                类----------表
                类中属性----------表中字段
                对象----------记录
            */
            const QByteArray data = serial->read(11);
            /*for (int k = 0; k <11; k++)
                qDebug()<<QString::number((uchar)data[k]);*/
            for (int j = 0; j < serialPortInfo->m_pDeviceList->size(); j++)
            {
                DeviceInfo info = serialPortInfo->m_pDeviceList->operator[](j);
                qDebug()<<"addr"<<QString::number((uchar)data[0]);
                qDebug()<<"info"<<QString::number(info.m_abyAddr[1]);
                if (info.m_abyAddr[1] == data[0])
                {
                    qDebug()<<"相等"<<data[0];
                }

                if (serialPortInfo->m_nDeviceType == THC)
                {
                    analysisTH(data, tOut, hOut);
                    ui->textEditTest->setText(serial->portName() + "温度:" + QString::number(tOut/100) + "." + QString::number(tOut%100) + "湿度:" + QString::number(hOut/100) + "." + QString::number(hOut%100));
                    ui->textEditDebug->setText(serial->portName() + "时间:" + QDateTime::currentDateTime().toString());
                    /* 插入数据库 */
                    if(!m_sqlQuery.exec("INSERT INTO TH3 VALUES('" + QDateTime::currentDateTime().toString("yyyy/M/d h:mm:s") +"', "+ QString::number(tOut) + ", " + QString::number(hOut) +  ")"))
                    {
                        qDebug() << m_sqlQuery.lastError();
                    }
                    else
                    {
                        qDebug() << "inserted value 1,25,25!";
                    }

                    obj->insert("温度", tOut);
                    obj->insert("湿度", hOut);
                }
                else if(serialPortInfo->m_nDeviceType == HM100PR)
                {
                    analysisHM100PR(data, tOut, pOut, cOut);
                    if (j < serialPortInfo->m_pDeviceList->size() - 1)
                    {
                        ui->textEditTest->append(serial->portName() + "(" + QString::number((uchar)info.m_abyAddr[1]) + ")温度:" + QString::number(tOut/100) + "." + QString::number(tOut%100)
                                                  + "压力:" + QString::number(pOut)
                                                  + "密度:" + QString::number(cOut));
                    }
                    else
                    {
                        ui->textEditTest->setText(serial->portName() + "(" + QString::number((uchar)info.m_abyAddr[1]) + ")温度:" + QString::number(tOut/100) + "." + QString::number(tOut%100)
                                                  + "压力:" + QString::number(pOut)
                                                  + "密度:" + QString::number(cOut));
                    }
                    ui->textEditDebug->setText(serial->portName() + "时间:" + QDateTime::currentDateTime().toString());
                    obj->insert("温度", tOut);
                    obj->insert("压力", pOut);
                    obj->insert("密度", cOut);

                    /* 插入数据库 */
                    if(!m_sqlQuery.exec("INSERT INTO TH015 VALUES('" + QDateTime::currentDateTime().toString("yyyy/M/d h:mm:s") +"', "+ QString::number(pOut) + ", " + QString::number(cOut) + ", " + QString::number(tOut) +  ")"))
                    {
                        qDebug() << m_sqlQuery.lastError();
                    }
                    else
                    {
                        qDebug() << "inserted value 015,25,25!";
                    }
                }
                /* 温湿度数据插入数据库后，发送信号更新表盘 */
                emit sendRtData(obj, serialPortInfo->m_nDeviceType);
            }
        }
        /* 读到的是设备地址 */
        else
        {
#if 0
            while(serial->bytesAvailable())
            {
                DeviceInfo deviceInfo;
                bool flag = false;
                const QByteArray adrData = serial->read(7);
                for(int i = 0; i < serialPortInfo->m_pDeviceList->size(); i++)
                {
                    /* 查找是否已有相同地址的表 */
                    if (serialPortInfo->m_pDeviceList->operator[](i).m_abyAddr[0] == adrData[3] &&
                            serialPortInfo->m_pDeviceList->operator[](i).m_abyAddr[1] == adrData[4])
                    {
                        flag = true;
                        break;
                    }
                }
                if (flag == false)
                {
                    deviceInfo.m_abyAddr[0] = adrData[3];
                    deviceInfo.m_abyAddr[1] = adrData[4];
                    serialPortInfo->m_pDeviceList->append(deviceInfo);
                    ui->textEditTest->append(serial->portName() + "添加新设备地址成功" + deviceInfo.m_abyAddr.toHex());
                }
            }
#endif
        }

        serialPortInfo->m_nWaitSerialCnt = 0;
        serialPortInfo->m_isChkAdrCmd = false;
    }
}

/*
    函数功能:定时给串口发送读数据消息
*/

void MainWindow::SendMsgFunc()
{
    for (int i = 0; i < m_pComlist->size(); i++)
    {
        QSerialPort *serial = m_pComlist->at(i).m_serial;
        SerialPortInfo portInfo = m_pComlist->at(i);
        unsigned short wCrc = 0;
        QByteArray abyd;
        uchar d[8];
        abyd.resize(8);
        for (int j = 0; j < portInfo.m_pDeviceList->size(); j++)
        {
            DeviceInfo deviceInfo = portInfo.m_pDeviceList->operator[](j);
            switch(portInfo.m_nDeviceType)
            {
                case THC:
                {
                    uchar td[8] = TH_CHK_DATA((uchar)deviceInfo.m_abyAddr[1], 2);
                    memcpy(d, td, 6);
                    break;
                }
                case HM100PR:
                {
                    uchar td[8] = HM_CHK_DATA((uchar)deviceInfo.m_abyAddr[1], 3);
                    memcpy(d, td, 6);
                    break;
                }

            }
            wCrc = Get_CRC(d, 6);
            d[7] = (wCrc&0xff00)>>8;
            d[6] = (wCrc&0x00ff);
            for (int k = 0; k < 8; k++)
            {
                abyd[k] = d[k];
            }
            if (serial->isOpen())
            {
                qDebug()<<d[0]<<d[1]<<d[2]<<d[3]<<d[4]<<d[5]<<d[6]<<d[7]<<QTime::currentTime();
                serial->write(abyd);
            }
            QTime t;
            t.start();
            while(t.elapsed()<100)
                QCoreApplication::processEvents();

        }
    }
    return ;
}

/*
    函数功能:开启定时器，定时向下位机发送读数据请求
*/
void MainWindow::on_pushButtonReadData_clicked()
{
    isRunFlag = !isRunFlag;
    if (m_timer->isActive())
    {
        disconnect(m_timer, SIGNAL(timeout()), this, SLOT(SendMsgFunc()));
        m_timer->stop();
        ui->textEditTest->append("定时读取关闭");
    }
    else
    {
        connect(m_timer, SIGNAL(timeout()), this, SLOT(SendMsgFunc()));
        m_timer->start(5*1000);
        ui->textEditTest->append("定时读取开启");
    }

}

/*
    函数功能:显示串口连接信息
*/
void MainWindow::on_pushButtonConnectInfo_clicked()
{
    ui->textEditDebug->append(QDateTime::currentDateTime().toString());
    ui->textEditDebug->append("COM连接数:" + QString::number(QSerialPortInfo::availablePorts().size()));
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->textEditDebug->append("Name:" + info.portName());
        ui->textEditDebug->append("Description : " + info.description());
        ui->textEditDebug->append("Manufacturer: " + info.manufacturer());
        ui->textEditDebug->append("Serial Number: " + info.serialNumber());
        ui->textEditDebug->append("System Location: " + info.systemLocation());
    }
}

/*
    函数功能:显示曲线图信息
*/

void MainWindow::on_pushButtonChartStatus_clicked()
{
    if (m_chartForm == nullptr)
    {
        m_chartForm = new ChartForm;
    }
    m_chartForm->show();

}
/*
    函数功能:显示表盘信息
*/
void MainWindow::on_pushButton_dashBoard_clicked()
{

    if (m_dBwdgt == nullptr)
    {
        m_dBwdgt = new DashBoardTabWidget;
        connect(this, SIGNAL(sendRtData(QJsonObject *, int)), m_dBwdgt, SLOT(rcvRtData(QJsonObject *, int)));

    }
    m_dBwdgt->show();

}
/*
    函数功能:显示数据表
*/

void MainWindow::on_pushButtonAllSheet_clicked()
{

    if (m_sheetForm == nullptr)
    {
        m_sheetForm = new DataSheetForm;
    }
    m_sheetForm->show();

}

/*
    函数功能:人工设置表类型
*/
void MainWindow::on_pushButton_setDeviceType_clicked()
{
    if (m_setForm == nullptr)
    {
        m_setForm = new SetDeviceTypeForm();
    }
    m_setForm->show();
    m_setForm->Create(m_pComlist);
}

/*
    函数功能:添加子设备
    note:串口连接对象可能是表也可能是集线器，集线器下会再连很多表，所以有时候
    需要往端口上添加子设备
*/

void MainWindow::on_pushButton_addChildDevice_clicked()
{
    if (m_addForm == nullptr)
    {
        m_addForm = new AddChildDeviceForm();
    }

    m_addForm->show();
    m_addForm->Create(m_pComlist);

}
