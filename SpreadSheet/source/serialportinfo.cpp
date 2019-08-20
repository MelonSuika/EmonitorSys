#include "serialportinfo.h"

SerialPortInfo::SerialPortInfo():
    m_serial(new QSerialPort()),
    m_serialInfo(new QSerialPortInfo())
{

    m_serial = nullptr;
    m_serialInfo = nullptr;

    m_isRunFlag = false;
    m_nWaitSerialCnt = 0;
    m_isChkAdrCmd = false;

    m_serial = new QSerialPort();
    m_serialInfo = new QSerialPortInfo();

    m_nDeviceType = 0;
    //connect(m_serial, SIGNAL(QSerialPort::errorOccurred()), this, SLOT(SerialPortInfo::errorFunc()));
    //QObject::connect(m_serial, SIGNAL(QSerialPort::errorOccurred(QSerialPort::SerialPortError)), this, SLOT(SerialPortInfo::errorFunc(QSerialPort::SerialPortError)));
}


void SerialPortInfo::setChkAdrCmd(bool value)
{
    m_isChkAdrCmd = value;
}

void SerialPortInfo::errorFunc(QSerialPort::SerialPortError error)
{
    if (m_serial->error() ==  QSerialPort::ResourceError)
    {
        //ui->textEditTest->setText("串口拔出");
        qDebug()<<m_serial->portName()<<"拔出";
    }

}
