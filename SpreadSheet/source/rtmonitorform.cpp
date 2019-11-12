#include "rtmonitorform.h"
#include "ui_rtmonitorform.h"

RtMonitorForm::RtMonitorForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RtMonitorForm)
{
    ui->setupUi(this);
}

RtMonitorForm::~RtMonitorForm()
{
    delete ui;
}
