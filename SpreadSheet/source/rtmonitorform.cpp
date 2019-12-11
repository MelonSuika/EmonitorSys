#include "rtmonitorform.h"
#include "ui_rtmonitorform.h"
#include "pagenavigator.h"

using namespace std;
RtMonitorForm::RtMonitorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RtMonitorForm)
{
    ui->setupUi(this);

    m_pListDash = new QList<SingleDashForm *>;
    m_nTestDashCount = 256;

    m_pPageNavigator = new PageNavigator(3);
    ui->verticalLayout_left->addWidget(m_pPageNavigator);

    connect(m_pPageNavigator, SIGNAL(currentPageChanged(int)), this, SLOT(upDatePageWidget(int)));

}

RtMonitorForm::~RtMonitorForm()
{
    delete ui;
}

void RtMonitorForm::rcvRtData(QJsonObject *data, DeviceSymbolInfo deviceSbInfo)
{

    int n;
    bool flag = false;
    int a = data->value("地址").toInt();
    for (int i = 0; i < m_pListDash->size(); i++)
    {
        SingleDashForm *dash = m_pListDash->operator[](i);
        if (dash->m_deviceSbInfo.nAddress == a)
        {
            flag = true;
            // 更新表盘
            dash->setDashData(data);
            break;
        }
    }

    if (flag == false)
    {
        SingleDashForm *dash = new SingleDashForm;
        dash->m_deviceSbInfo.nAddress = a;
        dash->setDashData(data);
        m_pListDash->append(dash);
        m_pPageNavigator->setMaxPage(m_pListDash->size()/PAGE_MAXCOUNT + 1);
        if (ui->girdLayoutMSheet->count() < PAGE_MAXCOUNT)
        {
            n = ui->girdLayoutMSheet->count();
            // 添加表盘
            ui->girdLayoutMSheet->addWidget(dash, n/PAGE_MAXCOLUMN, n%PAGE_MAXCOLUMN);
        }
    }
}


/*
    函数功能：slot，导航页当前页改变=>显示Widget改变
*/
void RtMonitorForm::upDatePageWidget(int page)
{

    qDebug()<<"updatePage"<<page;
    for (int i = 0; i < ui->girdLayoutMSheet->count(); i++)
    {
        ui->girdLayoutMSheet->itemAt(i)->widget()->hide();
    }

    while ((ui->girdLayoutMSheet->takeAt(0)) != 0)
    {

    }

    int nStart = (page - 1) * PAGE_MAXCOUNT;
    for (int i = nStart; i < min(nStart + PAGE_MAXCOUNT, m_pListDash->size()); i ++)
    {

        ui->girdLayoutMSheet->addWidget(m_pListDash->at(i), i/PAGE_MAXCOLUMN, i%PAGE_MAXCOLUMN);
        m_pListDash->at(i)->show();
    }

}

/*
    函数功能：移除控件测试
*/
void RtMonitorForm::on_pushButton_removeDash_clicked()
{

    int size = m_pListDash->size();
    if (size >= 1)
    {
        for (int i = 0; i < ui->girdLayoutMSheet->count(); i++)
        {
            if (ui->girdLayoutMSheet->itemAt(i)->widget()->objectName() == QString::number(m_nTestDashCount-1))
            {
                ui->girdLayoutMSheet->itemAt(i)->widget()->hide();
                ui->girdLayoutMSheet->takeAt(i);
                break;
            }
        }

        for (int i = 0; i < m_pListDash->size(); i++)
        {
            if (m_pListDash->at(i)->m_deviceSbInfo.nAddress == (m_nTestDashCount-1))
            {
                m_pListDash->at(i)->deleteLater();
                m_pListDash->removeAt(i);
                m_pPageNavigator->setMaxPage((m_pListDash->size()-1)/PAGE_MAXCOUNT + 1);
                m_nTestDashCount--;
                break;
            }
        }
    }
    upDatePageWidget(m_pPageNavigator->getCurrentPage());

}

/*
    函数功能：添加控件测试
*/
void RtMonitorForm::on_pushButton_addDash_clicked()
{
    SingleDashForm *dash = new SingleDashForm;
    dash->setObjectName(QString::number(m_nTestDashCount));
    dash->m_deviceSbInfo.nAddress = m_nTestDashCount;
    QJsonObject json;
    json.insert("地址", m_nTestDashCount++);
    dash->setDashData(&json);
    m_pListDash->append(dash);
    m_pPageNavigator->setMaxPage((m_pListDash->size()-1)/PAGE_MAXCOUNT + 1);

    if (ui->girdLayoutMSheet->count() < PAGE_MAXCOUNT)
    {
        ui->girdLayoutMSheet->addWidget(dash, (m_pListDash->size()-1)/PAGE_MAXCOLUMN, (m_pListDash->size()-1)%PAGE_MAXCOLUMN);
    }
}
