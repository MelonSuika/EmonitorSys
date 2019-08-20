#include "common.h"



void traversalCom(QStringList &str, int &num)
{
    num = 0;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        QSerialPort serial;
        serial.setPort(info);
        if(serial.open(QIODevice::ReadWrite))
        {
            str[num] = info.portName();
            num++;
            serial.close();
        }
    }
}


