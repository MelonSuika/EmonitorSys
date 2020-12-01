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

int comtext2DMJType(QString str)
{


    int retType = 0;
    if (str == "未设置类型(0)")
    {
        retType = DMJ_NONE;
    }
    else if(str == "45R(4)001")
    {
        retType = DMJ45PR_1;
    }
    else if (str == "60R(4)048")
    {
        retType = DMJ60PR_1;
    }
    else if(str == "100R(4)高压015")
    {
        retType = DMJHM100PR_1;
    }
    else if(str == "100R(4)普压009")
    {
        retType = DMJ100PR_1;
    }
    else if(str == "100RDH(9)微水")
    {
        retType = DMJ_RDH_1;
    }
    else if(str == "THC(8)温湿度控制器")
    {
        // retType = DMJ
    }
    else if(str == "100PRW_1(4)无线")
    {
        retType = DMJ100PRW_1;
    }
    else if(str == "100PRW_2(4)无线")
    {
        retType = DMJ100PRW_2;
    }
    return retType;

}

int Arg2DMJType(QString arg)
{
    int retType = 0;
    if (arg == "60PR")
    {
        retType = DMJ60PR_1;
    }
    else if(arg == "100PR")
    {
        retType = DMJ100PR_1;
    }
    else if(arg == "HM100PR")
    {
        retType = DMJHM100PR_1;
    }
    else if(arg == "RDH")
    {
        retType = DMJ_RDH_1;
    }
    else if(arg == "100PRW_1")
    {
        retType = DMJ100PRW_1;
    }
    else if (arg == "100PRW_2")
    {
        retType = DMJ100PRW_2;
    }
    return  retType;

}
