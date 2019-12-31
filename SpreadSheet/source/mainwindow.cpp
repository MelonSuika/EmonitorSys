 #include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QLayout"
#include "spreadsheet.h"
#include <QThread>
#include <QtConcurrent>
#include <windows.h>
#include <QTimer>
#include <QtQuickWidgets/QQuickWidget>
#include "chartform.h"
#include "datasheetform.h"
#include "setdevicetypeform.h"
#include "addchilddeviceform.h"
#include "gasrelaymonitorform.h"
#include <memory>
#include <QQmlEngine>
#include <QQmlContext>
#include <QEvent>
#include <QResizeEvent>
#include "myqquickwidget.h"
#include "loadqss.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pComlist(new QList<SerialPortInfo>())
{
    ui->setupUi(this);    

    /* 隐去部分控件 */
    ui->pushButton_setDeviceType->hide();

    isRunFlag           = false;
    m_delayTimer        = new QTimer(this);
    m_nComCount = 0;

    /* 加载qss改变界面风格 */
    LoadQss::loadQss(SKIN_ORANGE);

    /* textEditConnectInfo防止信息太多崩溃 */
    ui->textEditConnectInfo->document()->setMaximumBlockCount(100);

    /* COM监测相关初始化 */
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        LOG(INFO) << "Name:" << info.portName().toStdString() << endl
                  << "Description:"<<info.description().toStdString() <<endl
                  << "Manufacturer:"<<info.manufacturer().toStdString();

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
            m_nComCount++;
        }
    }
    if (m_nComCount == 0)
    {
        ui->comboBoxIsActiveCom->addItem("无端口连接");
        LOG(INFO) << "无端口连接";
    }

    /* 定时间隔设置下拉初始化 */

    /* 查询地址隐去 */
    ui->pushButtonWriteSerialPort->hide();


    /* 初始化指针变量 */
    m_view = nullptr;
    m_model = nullptr;

    m_dBwdgt = nullptr;
    m_sheetForm = nullptr;
    m_chartForm = nullptr;
    m_setForm = nullptr;
    m_addForm = nullptr;
    m_gasQuickForm = nullptr;
    m_aboutFrom = nullptr;
    m_nCount = 0;
    m_nReadTimeGap = 5000;
    m_nRcvCount = 0;
    m_nSendCount = 0;


    m_test = 0;
}


MainWindow::~MainWindow()
{
    delete ui;
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
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            if (serial ->open(QIODevice::ReadWrite))
            {
                mainWidgetPrint("Open serial " + serial->portName() + " success", textEditPrint);
                ui->pushButtonSerial->setText("关闭串口");
            }
            else
            {
                mainWidgetPrint("Open serial " + serial->portName() + " fail", textEditPrint);
            }
        }
        else
        {
            serial->close();
            ui->textEditPrint->append("close serial " + serial->portName() + " success");
            ui->pushButtonSerial->setText("打开串口");
        }
        nCntCom++;

    }
    if (0 == nCntCom)
    {
        ui->textEditPrint->append("无串口连接");
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
            ui->textEditPrint->append(portInfo.m_serial->portName() + "未设置协议类型");
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
            ui->textEditPrint->append(serial->portName() + " send to lower" + " " + abyd.toHex());
        }
        else
        {
            ui->textEditPrint->setText("串口 " + serial->portName() + " 未打开");
        }
    }
    if (m_pComlist->size() == 0)
    {
        ui->textEditPrint->append("无串口连接");
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
    m_isReadSuccess = false;

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
            for (int j = 0; j < serialPortInfo->m_pDeviceList->size(); j++)
            {
                DeviceInfo info = serialPortInfo->m_pDeviceList->operator[](j);

                DeviceSymbolInfo deviceSbInfo;
                deviceSbInfo.nComIndex = i;
                deviceSbInfo.nDeviceIndex = j;
                deviceSbInfo.nDeviceType = info.m_nDeviceType;
                qDebug()<<"addr"<<QString::number((uchar)data[0]);
                qDebug()<<"info"<<QString::number(info.m_abyAddr[1]);
                if (info.m_abyAddr[1] == data[0])
                {
                    qDebug()<<"相等"<<data[0];
                }

                if (serialPortInfo->m_nDeviceType == THC)
                {
                    analysisTH(data, tOut, hOut);
                    ui->textEditPrint->setText(serial->portName() + "温度:" + QString::number(tOut/100) + "." + QString::number(tOut%100) + "湿度:" + QString::number(hOut/100) + "." + QString::number(hOut%100));
                    ui->textEditConnectInfo->setText(serial->portName() + "时间:" + QDateTime::currentDateTime().toString());
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
                        ui->textEditPrint->append(serial->portName() + "(" + QString::number((uchar)info.m_abyAddr[1]) + ")温度:" + QString::number((double)tOut/100)
                                                  + "压力:" + QString::number(pOut)
                                                  + "密度:" + QString::number(cOut));
                    }
                    else
                    {
                        ui->textEditPrint->setText(serial->portName() + "(" + QString::number((uchar)info.m_abyAddr[1]) + ")温度:" + QString::number((double)tOut/100)
                                                  + "压力:" + QString::number(pOut)
                                                  + "密度:" + QString::number(cOut));
                    }
                    ui->textEditConnectInfo->setText(serial->portName() + "时间:" + QDateTime::currentDateTime().toString());
                    obj->insert("温度", tOut);
                    obj->insert("压力", pOut);
                    obj->insert("密度", cOut);
                    obj->insert("地址", (uchar)info.m_abyAddr[1]);

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
                emit sendRtData(obj, deviceSbInfo);
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
                    ui->textEditPrint->append(serial->portName() + "添加新设备地址成功" + deviceInfo.m_abyAddr.toHex());
                }
            }
#endif
        }

        serialPortInfo->m_nWaitSerialCnt = 0;
        serialPortInfo->m_isChkAdrCmd = false;
    }
}

void readData2(QSerialPort *serial)
{
    int size = 0;
    {

        const QByteArray data = serial->read(25);
        size += data.size();

    }
    qDebug()<<"outSize:"<<size;


}

/*
    函数功能: 读取指定设备数据
    实际上对于上位机，只有com口之分，指定的nIndex只是逻辑上
    每个com口中连接添加的设备序号
*/
void MainWindow::readDeviceData(DeviceSymbolInfo deviceSbInfo)
{

    int tOut = -1, hOut = -1, pOut = -1, cOut = -1;
    QSerialPort *serial = m_pComlist->at(deviceSbInfo.nComIndex).m_serial;
    SerialPortInfo *serialPortInfo = &m_pComlist->operator[](deviceSbInfo.nComIndex);
    DeviceInfo *pCurInfo = &serialPortInfo->m_pDeviceList->operator[](deviceSbInfo.nDeviceIndex);

    const QByteArray data = serial->readAll();
    m_nRcvCount++;
    ui->lineEdit_rcvAndSendCount->setText(QString("TX:%1 RX:%2").arg(m_nSendCount).arg(m_nRcvCount));

    if (data.size() == 0)
    {
        /* 读不到数据3次，继续下个设备 */
        if (pCurInfo->m_nReadErrorCount < 3)
        {
            pCurInfo->m_nReadErrorCount++;
            QTimer::singleShot(500, this, [=](){readDeviceData(deviceSbInfo);});
            ui->textEditPrint->append(QString("%1(%2)读取异常").arg(serial->portName()).arg((uchar)pCurInfo->m_abyAddr[1]));
        }
        else
        {
            QTimer::singleShot(0, this, [=](){SendMsgFunc(deviceSbInfo.nDeviceIndex+1);});
            pCurInfo->m_nReadErrorCount = 0;
        }
        return;
    }


    analysisHM100PR(data, tOut, pOut, cOut);

    ui->textEditPrint->append(QString("%1(%2)温度(℃):%3压力(MPa):%4密度(MPa):%5").arg(serial->portName()).arg((uchar)pCurInfo->m_abyAddr[1]).arg((float)tOut/100).arg((float)pOut/10000).arg((float)cOut/10000));
    ui->lineEdit_time->setText(QString("%1时间:%2").arg(serial->portName()).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")));
    QJsonObject obj;
    obj.insert("温度", tOut);
    obj.insert("压力", pOut);
    obj.insert("密度", cOut);
    obj.insert("地址", (uchar)pCurInfo->m_abyAddr[1]);

    /* 插入数据库 */
    if(!m_sqlQuery.exec(QString("INSERT INTO TH015 VALUES('%1', '%2', '%3', '%4')").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg((float)pOut/10000).arg((float)cOut/10000).arg((float)tOut/100)))
    {

        qDebug() << m_sqlQuery.lastError();
    }
    else
    {
        qDebug() << "inserted value 015,25,25!";
    }
    /* 温湿度数据插入数据库后，发送信号更新表盘 */
    emit sendRtData(&obj, deviceSbInfo);

    QTimer::singleShot(0, this, [=](){SendMsgFunc(deviceSbInfo.nDeviceIndex+1);});

}


/*
    函数功能:定时给串口发送读数据消息
    (后期更改为子线程调用)
*/

void MainWindow::SendMsgFunc(int nIndex)
{

    for (int i = 0; i < m_pComlist->size(); i++)
    {

        if (isRunFlag == false)
        {
            mainWidgetPrint("already quit SendMsgFunc", textEditDebug);
            return ;
        }

        DeviceSymbolInfo deviceSbInfo;
        deviceSbInfo.nComIndex = i;
        deviceSbInfo.nDeviceIndex = nIndex;

        QSerialPort *serial = m_pComlist->at(i).m_serial;
        SerialPortInfo portInfo = m_pComlist->at(i);
        unsigned short wCrc = 0;
        QByteArray abyd;
        uchar d[8];
        abyd.resize(8);
        if (!serial->isOpen())
        {
            mainWidgetPrint(QString("串口(%1)已关闭").arg(serial->portName()), textEditPrint);
            continue;
        }

        /* If the nIndex exceeds max size, send from 0 */
        if (nIndex >= portInfo.m_pDeviceList->size())
        {
            QTimer::singleShot(0, this, [=](){SendMsgFunc(0);});
            return;
        }
        else
        {
            DeviceInfo deviceInfo = portInfo.m_pDeviceList->operator[](nIndex);
            deviceSbInfo.nAddress = deviceInfo.m_abyAddr[1];
            switch(deviceInfo.m_nDeviceType)
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
                serial->write(abyd);
                m_nSendCount++;
                QTimer::singleShot(1000, this,[=](){this->readDeviceData(deviceSbInfo);} );

            }
        }

    }
    return ;
}


/*
    函数功能:开启定时器，定时向下位机发送读数据请求
*/
void MainWindow::on_pushButtonReadData_clicked()
{
    if (isRunFlag)
    {
        isRunFlag = false;
        mainWidgetPrint("定时读取关闭", textEditPrint);
        ui->pushButtonReadData->setText("定时读取开启");
    }
    else
    {
        isRunFlag = true;
        QTimer::singleShot(0, this, SLOT(SendMsgFunc()));
        mainWidgetPrint("定时读取开启", textEditPrint);
        ui->pushButtonReadData->setText("定时读取关闭");
    }

}

/*
    函数功能:显示串口连接信息
*/
void MainWindow::on_pushButtonConnectInfo_clicked()
{
    mainWidgetPrint(QDateTime::currentDateTime().toString(), textEditConnectInfo);
    mainWidgetPrint("COM连接数:" + QString::number(QSerialPortInfo::availablePorts().size()), textEditConnectInfo);
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        mainWidgetPrint("Name:" + info.portName(), textEditConnectInfo);
        mainWidgetPrint("Description : " + info.description(), textEditConnectInfo);
        mainWidgetPrint("Manufacturer: " + info.manufacturer(), textEditConnectInfo);
        mainWidgetPrint("Serial Number: " + info.serialNumber(), textEditConnectInfo);
        mainWidgetPrint("System Location: " + info.systemLocation(), textEditConnectInfo);
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
        connect(this, SIGNAL(sendRtData(QJsonObject *, DeviceSymbolInfo)), m_dBwdgt, SLOT(rcvRtData(QJsonObject *, DeviceSymbolInfo)));

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
    note:串口连接对象可能是具体设备也可能是集线器，集线器下会再连很多设备，所以有时候
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

/*
    函数功能:设置读取间隔时间
*/
void MainWindow::on_pushButton_readSet_clicked()
{
    float fRet = 0;
    fRet = ui->lineEdit_readSet->text().toFloat();
    if (fRet == 0)
    {
        mainWidgetPrint("设置时间格式错误", textEditPrint);
    }
    else if(0.5>fRet || fRet>60)
    {
        mainWidgetPrint("设置时间范围出错 实际表数*(0.5~60)", textEditPrint);
    }
    else
    {
        m_nReadTimeGap = fRet*1000;
        mainWidgetPrint("重设间隔为:" + QString::number(fRet) + "秒", textEditPrint);
        on_pushButtonReadData_clicked();
        on_pushButtonReadData_clicked();
    }

}
/*
    函数功能:设置gas qml中的属性
*/
#if 1
void MainWindow::setQuickWidgetContextProperty()
{
    m_gasQuickForm->engine()->rootContext()->setContextProperty("quickWidth", m_gasQuickForm->width());
    m_gasQuickForm->engine()->rootContext()->setContextProperty("quickHeight", m_gasQuickForm->height());

}
#endif

/*
    函数功能:瓦斯继电器监测
*/
void MainWindow::on_pushButton_clicked()
{
#if 0
    if (m_gasForm == nullptr)
    {
        m_gasForm = new GasRelayMonitorForm;
    }
    m_gasForm->show();
#endif
    if (m_gasQuickForm == nullptr)
    {
        m_gasQuickForm = new MyQQuickWidget;
        m_gasQuickForm->engine()->rootContext()->setContextProperty("gasValue", 0);
        m_gasQuickForm->setFixedSize(800, 570);
        m_gasQuickForm->setWindowTitle("瓦斯继电器智能监测系统");
        m_gasQuickForm->engine()->rootContext()->setContextProperty("quickWidth", m_gasQuickForm->width());
        m_gasQuickForm->engine()->rootContext()->setContextProperty("quickHeight", m_gasQuickForm->height());
        m_gasQuickForm->setSource(QUrl("qrc:/gas.qml"));

        connect(m_gasQuickForm, SIGNAL(setQuickWidgetContext()), this, SLOT(setQuickWidgetContextProperty()));

        m_gasQuickForm->startRcv();


    }
    m_gasQuickForm->show();

}

void MainWindow::on_actionblue_triggered()
{
    LoadQss::loadQss(SKIN_BLUE);
}

void MainWindow::on_actionblack_triggered()
{
    LoadQss::loadQss(SKIN_DARK);
}

void MainWindow::on_actionOrange_triggered()
{
    LoadQss::loadQss(SKIN_ORANGE);
}

void MainWindow::on_actionabout_triggered()
{
    if (nullptr == m_aboutFrom)
    {
        m_aboutFrom = new AboutForm();
    }
    m_aboutFrom->show();

}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{

    if (eventType == QByteArray("windows_generic_MSG"))
    {
        MSG *pMsg = reinterpret_cast<MSG*>(message);
        if(pMsg->message == WM_DEVICECHANGE)
        {
            switch(pMsg->wParam)
            {
            // USB设备连上
            case /*DBT_DEVICEARRIVAL*/0x8000:
                mainWidgetPrint("有新端口接入", textEditDebug);
                upDateComComboBox();
                break;
            // USB设备断开
            case /*DBT_DEVICEREMOVECOMPLETE*/0x8004:
                mainWidgetPrint("有端口断开", textEditDebug);
                upDateComComboBox();
                break;
            //其他的消息可以查看“Dbt.h”文件
            default:
                break;
            }
        }

    }
    return QWidget::nativeEvent(eventType, message, result);
}


/*
    函数功能：打印输出至主界面窗口选择
*/
void MainWindow::mainWidgetPrint(QString info, int outPutWidget)
{
    switch (outPutWidget) {
        case textEditPrint:
            ui->textEditPrint->append(info);
            break;
        case textEditDebug:
            ui->textEditRcvDisplay->append(info);
            break;
        case textEditConnectInfo:
            ui->textEditConnectInfo->append(info);
            break;
        default:
            break;
    }

}

/*
    函数功能: 更新主界面的ComComboBox
*/
void MainWindow::upDateComComboBox()
{
    int nCount = 0;
    ui->comboBoxIsActiveCom->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        nCount++;
        ui->comboBoxIsActiveCom->addItem(info.portName());
    }
    if (nCount == 0)
    {
        ui->comboBoxIsActiveCom->addItem("无连接端口");
    }

}


