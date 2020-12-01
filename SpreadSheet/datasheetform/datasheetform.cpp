#include "datasheetform.h"
#include "ui_datasheetform.h"
#include <QtQuickWidgets/QQuickWidget>
#include <QCalendarWidget>
#include <QSqlQuery>
#include <QScrollBar>

#define LINESPPAGE 1000

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
    m_nCurrentPage = 1;

    m_strCurrentSimpleQuery = QString("select * from TH015A ");
    m_strCurrentQuery = m_strCurrentSimpleQuery + "limit '%1', '%2'";
    m_model->setQuery(m_strCurrentQuery.arg(0).arg(LINESPPAGE));

    m_model->setHeaderData(0, Qt::Horizontal, tr("时间"));
    m_model->setHeaderData(1, Qt::Horizontal, tr("压力(KPa)"));
    m_model->setHeaderData(2, Qt::Horizontal, tr("密度(g/L)"));
    m_model->setHeaderData(3, Qt::Horizontal, tr("温度(℃)"));
    m_model->setHeaderData(4, Qt::Horizontal, tr("地址"));


    ui->tableView_Sheet->setModel(m_model);
    qDebug()<<ui->tableView_Sheet;

    /* 设置视图大小 */
    ui->tableView_Sheet->setColumnWidth(0, 180);

    /* 激活日历框 */
    ui->dateTimeEdit_start->setCalendarPopup(true);
    ui->dateTimeEdit_end->setCalendarPopup(true);

    /* 设置为当前时间 */
    ui->dateTimeEdit_start->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());

    /* 页码临时更新 */
    upDatePageInfo();

}

DataSheetForm::~DataSheetForm()
{
    delete ui;
}

void DataSheetForm::on_pushButton_query_clicked()
{
    m_timeLastSql.start();
    if (ui->lineEdit_address->text() == "")
    {
        m_strCurrentSimpleQuery = QString("SELECT * FROM TH015AD WHERE time > '%1' and time < '%2' ").
                arg(ui->dateTimeEdit_start->text()).arg(ui->dateTimeEdit_end->text());
        m_strCurrentQuery = m_strCurrentSimpleQuery + "limit '%1', '%2'";

        m_model->setQuery(m_strCurrentQuery.arg(0).arg(LINESPPAGE));
    }
    else
    {
        m_strCurrentSimpleQuery = QString("SELECT * FROM TH015AD WHERE time > '%1' and time < '%2'and address == '%3' ").
                arg(ui->dateTimeEdit_start->text()).arg(ui->dateTimeEdit_end->text()).arg(ui->lineEdit_address->text());
        m_strCurrentQuery = m_strCurrentSimpleQuery + "limit '%1', '%2'";
        m_model->setQuery(m_strCurrentQuery.arg(0).arg(LINESPPAGE));
    }
    upDatePageInfo();

}

/*
    函数功能:上一页
*/
void DataSheetForm::on_pushButton_previousPage_clicked()
{
    m_timeLastSql.start();
    if (m_nCurrentPage <= 1)
    {
        return;
    }
    m_model->setQuery(m_strCurrentQuery.arg(LINESPPAGE*(m_nCurrentPage-2)).arg(LINESPPAGE));
    m_nCurrentPage--;
    upDatePageInfo();

}

/*
    函数功能:下一页
*/
void DataSheetForm::on_pushButton_nextPage_clicked()
{

    m_timeLastSql.start();
    if (m_nCurrentPage >= m_nPageCount)
    {
        return;
    }
    m_model->setQuery(m_strCurrentQuery.arg(LINESPPAGE*m_nCurrentPage).arg(LINESPPAGE));

    m_nCurrentPage++;
    upDatePageInfo();

}

/*
    函数功能:更新状态栏
*/
void DataSheetForm::upDatePageInfo()
{
    QSqlQuery query(m_strCurrentSimpleQuery);
    query.last();
    int nLastIndex = query.at();
    nLastIndex = nLastIndex >= 0? nLastIndex + 1 : 0;
    m_nPageCount = nLastIndex/LINESPPAGE + 1;
    ui->label_pageCount->setText(QString("共%1页").arg(m_nPageCount));
    ui->label_currentPage->setText(QString("第%1页").arg(m_nCurrentPage));
    ui->lineEdit_timeCosume->setText("操作耗时:" + QString::number(m_timeLastSql.elapsed()) + "ms");
    ui->lineEdit_CountPerPage->setText("每页记录数:" + QString::number(LINESPPAGE));
    ui->lineEdit_documentCount->setText("全部记录数:" + QString::number(nLastIndex));
    ui->spinBox_gotoPage->setMaximum(m_nPageCount);

}

/*
    函数功能:返回第一页
*/
void DataSheetForm::on_pushButton_firstPage_clicked()
{
    m_timeLastSql.start();
    m_model->setQuery(m_strCurrentQuery.arg(LINESPPAGE*0).arg(LINESPPAGE));
    m_nCurrentPage = 1;
    upDatePageInfo();
}

/*
    函数功能:最后一页
*/
void DataSheetForm::on_pushButton_lastPage_clicked()
{
    m_timeLastSql.start();
    m_model->setQuery(m_strCurrentQuery.arg(LINESPPAGE*(m_nPageCount-1)).arg(LINESPPAGE));
    m_nCurrentPage = m_nPageCount;
    upDatePageInfo();
}

/*
    函数功能:跳转页
*/
void DataSheetForm::on_pushButton_gotoPage_clicked()
{
    m_timeLastSql.start();
    int nPage = ui->spinBox_gotoPage->value();
    m_model->setQuery(m_strCurrentQuery.arg(LINESPPAGE*(nPage-1)).arg(LINESPPAGE));
    m_nCurrentPage = nPage;
    upDatePageInfo();
}

/*
    函数功能:打印
*/
void DataSheetForm::on_pushButton_print_clicked()
{
    QPrinter printer;
    QString printerName = printer.printerName();
    //if (printerName.size() == 0)

    qDebug()<<printerName;
}
