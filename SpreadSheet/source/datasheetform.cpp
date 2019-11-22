#include "datasheetform.h"
#include "ui_datasheetform.h"
#include <QtQuickWidgets/QQuickWidget>

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

    m_model->setQuery("select * from TH015");
    m_model->setHeaderData(0, Qt::Horizontal, tr("时间"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("压力"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("密度"));
    m_model->setHeaderData(3, Qt::Horizontal, tr("温度"));


    ui->tableView_Sheet->setModel(m_model);

    /* 设置视图大小 */
    ui->tableView_Sheet->setColumnWidth(0, 180);

}

DataSheetForm::~DataSheetForm()
{
    delete ui;
}

void DataSheetForm::on_pushButton_query_clicked()
{

    qDebug() << ui->dateTimeEdit_start->text() <<endl <<ui->dateTimeEdit_end->text()<<endl<<"SELECT time, pressure, density, temperature FROM TH015\
                WHERE time between '" + ui->dateTimeEdit_start->text() + "' and '" + ui->dateTimeEdit_end->text() + "';";


    m_model->setQuery("SELECT time, pressure, density, temperature FROM TH015\
                WHERE time > '" + ui->dateTimeEdit_start->text() + "' and time < '" + ui->dateTimeEdit_end->text() + "';");

    m_model->setQuery("SELECT time, pressure, density, temperature FROM TH015\
                WHERE time between '" + ui->dateTimeEdit_start->text() + "' and '" + ui->dateTimeEdit_end->text() + "' order by time desc ");



}
