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
#include <memory>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pComlist(new QList<SerialPortInfo>())
{
    ui->setupUi(this);    

    isRunFlag           = false;
    m_timer             = new QTimer(this);

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
            connect(serialPortInfo.m_serial, &QSerialPort::readyRead, this, &MainWindow::readData);
            connect(serialPortInfo.m_serial, SIGNAL(QSerialPort::errorOccurred(QSerialPort::SerialPortError)), this, SLOT(MainWindow::errorFunc(QSerialPort::SerialPortError)));
            m_nComCount++;
        }
    }

    /* 初始化指针变量 */
    m_view = nullptr;
    m_model = nullptr;

    m_dBwdgt = nullptr;
    m_sheetForm = nullptr;
    m_chartForm = nullptr;
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
*/
void MainWindow::on_pushButtonWriteSerialPort_clicked()
{
    for (int i = 0; i < m_pComlist->size(); i++)
    {
        QSerialPort *serial = m_pComlist->at(i).m_serial;
        m_pComlist->operator[](i).m_isChkAdrCmd = true;
        if (serial->isOpen())
        {
            unsigned short wCrc = 0;
            QByteArray abyd;
            unsigned char d[8] = TH_CHK_ADDR(0x00, 0x01);
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
            ui->textEditTest->setText(serial->portName() + ": " + abyd.toHex());
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
    tOut = dataIn[3]*16*16 + dataIn[4];
    hOut = dataIn[5]*16*16 + dataIn[6];
}


/*
    函数功能:串口读数据
*/
void MainWindow::readData()
{

    for (int i = 0; i < m_pComlist->size(); i++)
    {
        int tOut = -1, hOut = -1;
        QSerialPort *serial = m_pComlist->at(i).m_serial;
        SerialPortInfo *serialPortInfo = &m_pComlist->operator[](i);
        /* 如果是第一次查询地址后的第一次读取，需要判断缓存是否等于7 */
        if (serialPortInfo->m_isChkAdrCmd == true && serial->bytesAvailable() != 7)
        {
            serialPortInfo->m_nWaitSerialCnt++;
            return;
        }
        /* 如果不是查询后的第一次读取，那就是温湿度读取，判断是否大于9 */
        if (serialPortInfo->m_isChkAdrCmd == false && serial->bytesAvailable() != 9)
        {
            serialPortInfo->m_nWaitSerialCnt++;
            return;
        }

        const QByteArray data = serial->readAll();
        ui->textEditRcvDisplay->setText(data.toHex());
        if (serialPortInfo->m_isChkAdrCmd == false)
        {

            /*
                数据库中以表为组织单位存储数据
                表类似我们的Java类，每个字段都有对应的数据类型。
                那么用我们熟悉的java程序来与关系型数据对比，就会发现以下对应关系。
                类----------表
                类中属性----------表中字段
                对象----------记录

            */
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

            /* 温湿度数据插入数据库后，发送信号更新表盘 */
            emit sendRtData(tOut);
        }
        /* 读到的是设备地址 */
        else
        {
            serialPortInfo->m_abyAddr[0] = data[3];
            serialPortInfo->m_abyAddr[1] = data[4];
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
        unsigned short wCrc = 0;
        QByteArray abyd;
        abyd.resize(8);
        //uchar d[8] = {0xff, 0x03, 0x00, 0x00, 0x00, 0x02};
        uchar d[8] = TH_CHK_DATA((uchar)m_pComlist->at(i).m_abyAddr[1], 2);
        wCrc = Get_CRC(d, 6);
        d[7] = (wCrc&0xff00)>>8;
        d[6] = (wCrc&0x00ff);
        for (int i = 0; i < 8; i++)
        {
            abyd[i] = d[i];
        }
        if (serial->isOpen())
        {
            serial->write(abyd);
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
        connect(this, SIGNAL(sendRtData(int)), m_dBwdgt, SLOT(rcvRtData(int)));

    }
    m_dBwdgt->show();
    /*std::unique_ptr<DashBoardTabWidget> dBwdgt(new DashBoardTabWidget);
    dBwdgt->show();*/

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

    SetDeviceTypeForm *setForm = new SetDeviceTypeForm();
    setForm->show();


}
