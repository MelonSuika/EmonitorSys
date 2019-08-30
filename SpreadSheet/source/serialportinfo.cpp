#include "serialportinfo.h"

SerialPortInfo::SerialPortInfo():
    m_serial(new QSerialPort()),
    m_serialInfo(new QSerialPortInfo()),
    m_pDeviceList(new QList<DeviceInfo>())
{

    m_serial = nullptr;
    m_serialInfo = nullptr;

    m_isRunFlag = false;
    m_nWaitSerialCnt = 0;
    m_isChkAdrCmd = false;

    m_serial = new QSerialPort();
    m_serialInfo = new QSerialPortInfo();

    m_nDeviceType = TYPE_NONE;

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
        qDebug()<<m_serial->portName()<<"拔出";
    }

}
