#ifndef DEVICEINFO_H
#define DEVICEINFO_H

#include "common.h"

class DeviceInfo
{
public:
    DeviceInfo();

    QByteArray m_abyAddr;          /* 设备地址 */
    int m_nDeviceType;             /* 设备类型 */

    int m_nReadErrorCount;         /* 读取异常次数 */

};

#endif // DEVICEINFO_H
