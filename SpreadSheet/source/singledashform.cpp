#include "singledashform.h"
#include "ui_singledashform.h"
#include <QJsonObject>

SingleDashForm::SingleDashForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleDashForm)
{
    ui->setupUi(this);

    /* 加载表盘 */
    /*
    ui->quickDashWidget->setSource(QUrl("qrc:/dialcontrol.qml"));
    ui->quickDashWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    ui->quickDashWidget->show();
    */

    //m_quickWidget = new QQuickWidget;
    //m_quickWidget->setSource(QUrl(""));
    //ui->verticalLayout_display->addWidget(m_quickWidget);

}

SingleDashForm::~SingleDashForm()
{
    delete ui;
}

void SingleDashForm::setDashData(QJsonObject *data)
{
    int t = data->value("温度").toInt();
    int p = data->value("压力").toInt();
    int c = data->value("密度").toInt();
    int a = data->value("地址").toInt();

    ui->lineEdit_address->setText(QString::number(data->value("地址").toInt()));
    ui->lineEdit_temperature->setText(QString::asprintf("%.2f", (float)t/100));
    ui->lineEdit_pressure->setText(QString::asprintf("%.3f", (float)p/10000 - 0.1));
    ui->lineEdit_density->setText(QString::asprintf("%.3f", (float)c/10000 - 0.1));


}
