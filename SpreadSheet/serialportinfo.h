#ifndef SERIALPORTINFO_H
#define SERIALPORTINFO_H
#include "common.h"

class SerialPortInfo
{
public:
    SerialPortInfo();
    void setChkAdrCmd(bool value);

    QSerialPort *m_serial;
    QSerialPortInfo *m_serialInfo;

    bool m_isRunFlag;
    QByteArray m_abyBuffer;     /* 接收数据备份缓存 */
    int m_nWaitSerialCnt;       /* 缓存区数据不够计数 */
    bool m_isChkAdrCmd;         /* 查询地址命令 */

    int m_nDeviceType;

private slots:
    void errorFunc(QSerialPort::SerialPortError error/*QSerialPort serial*/);

};

#endif // SERIALPORTINFO_H
