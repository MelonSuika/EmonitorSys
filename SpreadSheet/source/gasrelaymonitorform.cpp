#include "gasrelaymonitorform.h"
#include "ui_gasrelaymonitorform.h"
#include <QFile>

GasRelayMonitorForm::GasRelayMonitorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GasRelayMonitorForm)
{
    ui->setupUi(this);
    /* 加载qss改变界面风格 */
    QFile qssfile(":/qss/widget-blue.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    this->setStyleSheet(qss);

    /* 加载浮子UI */
    m_quickWidget = new QQuickWidget;
    m_quickWidget->setSource(QUrl("qrc:/gas.qml"));
    //ui->verticalLayout_dashBoard->addWidget(m_quickWidget);
    ui->verticalLayout_display->addWidget(m_quickWidget);

    /* 固定大小 */
    m_quickWidget->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    m_quickWidget->setFixedSize(625, 600);

}

GasRelayMonitorForm::~GasRelayMonitorForm()
{
    delete ui;
}
