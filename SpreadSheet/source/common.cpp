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


int comtextToType(QString str)
{
    QString num = "";
    bool flag = false;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == ")")
        {
            break;
        }
        if (flag)
        {
            num += str[i];
        }
        if (str[i] == "(")
        {
            flag = true;
        }

    }
    return num.toInt();

}
