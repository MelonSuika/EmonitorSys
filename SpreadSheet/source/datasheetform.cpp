#include "datasheetform.h"
#include "ui_datasheetform.h"
#include <QtQuickWidgets/QQuickWidget>

DataSheetForm::DataSheetForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataSheetForm)
{
    ui->setupUi(this);
    /* 加载qss改变界面风格 */
    QFile qssfile(":/qss/widget-blue.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    this->setStyleSheet(qss);


    m_model = nullptr;

    if (m_model != nullptr)
    {
        delete m_model;
        m_model = nullptr;
    }
    m_model = new QSqlQueryModel;

    m_model->setQuery("select * from TH3");
    m_model->setHeaderData(0, Qt::Horizontal, tr("时间"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("温度"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("湿度"));

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

    qDebug() << ui->dateTimeEdit_start->text() <<endl <<ui->dateTimeEdit_end->text()<<endl<<"SELECT time, temperature, humidity FROM TH3\
                WHERE time > '" + ui->dateTimeEdit_start->text() + "' and time < '" + ui->dateTimeEdit_end->text() + "';";


    m_model->setQuery("SELECT time, temperature, humidity FROM TH3\
                WHERE time > '" + ui->dateTimeEdit_start->text() + "' and time < '" + ui->dateTimeEdit_end->text() + "';");



}
