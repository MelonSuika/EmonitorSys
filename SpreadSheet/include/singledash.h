#ifndef SINGLEDASH_H
#define SINGLEDASH_H

#include "singledashform.h"
#include "common.h"

class SingleDash
{
public:
    SingleDash();

    SingleDashForm *m_pDashForm;
    DeviceSymbolInfo m_deviceSbInfo;

    void setDashData(QJsonObject data);
};

#endif // SINGLEDASH_H
