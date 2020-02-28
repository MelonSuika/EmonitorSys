#ifndef DATASHEETFORM_H
#define DATASHEETFORM_H

#include <QWidget>
#include "common.h"


namespace Ui {
class DataSheetForm;
}

class DataSheetForm : public QWidget
{
    Q_OBJECT

public:
    explicit DataSheetForm(QWidget *parent = nullptr);
    ~DataSheetForm();

private slots:
    void on_pushButton_query_clicked();

    void on_pushButton_previousPage_clicked();

    void on_pushButton_nextPage_clicked();

    void upDatePageInfo();

    void on_pushButton_firstPage_clicked();

    void on_pushButton_lastPage_clicked();

    void on_pushButton_gotoPage_clicked();

    void on_pushButton_print_clicked();

private:
    Ui::DataSheetForm *ui;

    QSqlQueryModel *m_model;

    int m_nCurrentPage;
    int m_nPageCount;
    QString m_strCurrentQuery;
    QString m_strCurrentSimpleQuery;
    QTime m_timeLastSql;
};

#endif // DATASHEETFORM_H
