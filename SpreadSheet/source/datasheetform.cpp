#include "datasheetform.h"
#include "ui_datasheetform.h"
#include <QtQuickWidgets/QQuickWidget>
#include <QCalendarWidget>

DataSheetForm::DataSheetForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataSheetForm)
{
    ui->setupUi(this);

    m_model = nullptr;

    if (m_model != nullptr)
    {
        delete m_model;
        m_model = nullptr;
    }
    m_model = new QSqlQueryModel;

    m_model->setQuery(QString("select * from TH015 limit '%1'").arg(1000));
    m_model->setHeaderData(0, Qt::Horizontal, tr("时间"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("压力(KPa)"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("密度(g/L)"));
    m_model->setHeaderData(3, Qt::Horizontal, tr("温度(℃)"));


    ui->tableView_Sheet->setModel(m_model);

    /* 设置视图大小 */
    ui->tableView_Sheet->setColumnWidth(0, 180);

    /* 激活日历框 */
    ui->dateTimeEdit_start->setCalendarPopup(true);
    ui->dateTimeEdit_end->setCalendarPopup(true);

    /* 设置为当前时间 */
    ui->dateTimeEdit_start->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());

}

DataSheetForm::~DataSheetForm()
{
    delete ui;
}

void DataSheetForm::on_pushButton_query_clicked()
{
    qDebug()<<ui->dateTimeEdit_start;
    m_model->setQuery(QString("SELECT * FROM TH015 WHERE time > '%1' and time < '%2' limit '%3'").arg(ui->dateTimeEdit_start->text()).arg(ui->dateTimeEdit_end->text()).arg(10000));

}

