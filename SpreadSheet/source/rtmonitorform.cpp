#include "rtmonitorform.h"
#include "ui_rtmonitorform.h"
#include "pagenavigator.h"

RtMonitorForm::RtMonitorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RtMonitorForm)
{
    ui->setupUi(this);

    m_pListDash = new QList<SingleDash>;
    m_nDashCount = 0;

    PageNavigator *p = new PageNavigator(3);
    ui->verticalLayout_left->addWidget(p);

}

RtMonitorForm::~RtMonitorForm()
{
    delete ui;
}

void RtMonitorForm::rcvRtData(QJsonObject *data, DeviceSymbolInfo deviceSbInfo)
{

    bool flag = false;
    int t = data->value("温度").toInt();
    int p = data->value("压力").toInt();
    int c = data->value("密度").toInt();
    int a = data->value("地址").toInt();
    for (int i = 0; i < m_pListDash->size(); i++)
    {
        SingleDash dash = m_pListDash->operator[](i);

        if (dash.m_deviceSbInfo.nAddress == a)
        {
            flag = true;
            // 更新表盘
            dash.setDashData(*data);
        }

    }
    if (flag == false)
    {
        SingleDash dash;
        dash.m_deviceSbInfo.nAddress = a;
        dash.setDashData(*data);

        m_pListDash->append(dash);
        // 添加表盘
        ui->girdLayoutMSheet->addWidget(dash.m_pDashForm, m_nDashCount/5, m_nDashCount%5);
        m_nDashCount++;

    }


}

void RtMonitorForm::on_pushButton_clicked()
{
    SingleDash dash;
    ui->girdLayoutMSheet->addWidget(dash.m_pDashForm, m_nDashCount/5, m_nDashCount%5);
    m_nDashCount++;
}
