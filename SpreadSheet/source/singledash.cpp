#include "singledash.h"

SingleDash::SingleDash()
{
    m_pDashForm = new SingleDashForm;
    m_deviceSbInfo.nComIndex = -1;
    m_deviceSbInfo.nDeviceIndex = -1;
}

void SingleDash::setDashData(QJsonObject data)
{

    m_pDashForm->setDashData(&data);

}
