#include "curveform.h"
#include "ui_curveform.h"

CurveForm::CurveForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CurveForm)
{
    ui->setupUi(this);
    connect(this, SIGNAL(sigSend2Chart(QJsonObject *, DeviceSymbolInfo)), ui->widget, SLOT(rcvRtData(QJsonObject *, DeviceSymbolInfo)));
    connect(this, SIGNAL(sigPassThrough(QString)), ui->widget, SLOT(slotAddr2Arg(QString)));
}

CurveForm::~CurveForm()
{
    delete ui;
}

void CurveForm::slotRcvRtData(QJsonObject *data, DeviceSymbolInfo info)
{
    emit sigSend2Chart(data, info);

}
