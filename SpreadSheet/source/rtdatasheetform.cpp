#include "rtdatasheetform.h"
#include "ui_rtdatasheetform.h"

RtDataSheetForm::RtDataSheetForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RtDataSheetForm)
{
    ui->setupUi(this);

    //ui->tableWidget;

    ui->tableWidget->setRowCount(20); //设置行数为10
    ui->tableWidget->setColumnCount(5);

    /* 设置列名 */
    QStringList headers;
    headers<< "表号"<<"温度"<<"压力"<<"密度"<<"位置";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

}

RtDataSheetForm::~RtDataSheetForm()
{
    delete ui;
}
