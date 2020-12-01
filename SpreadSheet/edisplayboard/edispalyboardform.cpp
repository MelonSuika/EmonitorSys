#include "edispalyboardform.h"
#include "ui_edispalyboardform.h"
#include <QDebug>
#include <QMouseEvent>

EDispalyBoardForm::EDispalyBoardForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EDispalyBoardForm)
{
    ui->setupUi(this);
    /* 隐去默认标题栏 */
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

#if 0
    ui->widget_LeftUpConnectForm;
    ui->widget_RightUpMulDMJForm;
    QSplitter *splitter = new QSplitter(parent);
    splitter->addWidget(ui->widget_LeftUpConnectForm);
    splitter->addWidget(ui->widget_RightUpMulDMJForm);
#endif
    qDebug()<<"EDispalyBoardForm";
    /* 打开即全屏 */
    this->showFullScreen();


    /* 信号分发到子界面 */
    connect(this, SIGNAL(sigSend2SubForm(QJsonObject *, DeviceSymbolInfo)), ui->widget_LeftDownSingleDMJForm, SLOT(slotRcvRtData(QJsonObject *, DeviceSymbolInfo)));
    connect(this, SIGNAL(sigSend2SubForm(QJsonObject *, DeviceSymbolInfo)), ui->widget_RightUpMulDMJForm, SLOT(slotRcvRtData(QJsonObject *, DeviceSymbolInfo)));
    connect(this, SIGNAL(sigSend2SubForm(QJsonObject *, DeviceSymbolInfo)), ui->widget_RightDownCurveForm, SLOT(slotRcvRtData(QJsonObject *, DeviceSymbolInfo)));
    connect(ui->widget_LeftUpConnectForm, SIGNAL(sigDisplayChange(QString)),
            ui->widget_LeftDownSingleDMJForm, SLOT(slotChangeCombText(QString)));
    connect(ui->widget_LeftUpConnectForm, SIGNAL(sigDisplayChange(QString)),
            ui->widget_RightDownCurveForm, SIGNAL(sigPassThrough(QString)));

}

EDispalyBoardForm::~EDispalyBoardForm()
{
    delete ui;
}

void EDispalyBoardForm::slotRcvMainEp(QJsonObject *obj, DeviceSymbolInfo info)
{

    int t = obj->value("温度").toInt();
    int p = obj->value("压力").toInt();
    int c = obj->value("密度").toInt();
    qDebug()<<"second data:t, p, c, address, DMJType"<<t<<p<<c<<info.nAddress<<info.nDMJType;
    emit sigSend2SubForm(obj, info);

}
