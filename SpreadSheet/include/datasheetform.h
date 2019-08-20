#ifndef DATASHEETFORM_H
#define DATASHEETFORM_H

#include <QWidget>
#include <QSqlQueryModel>
#include <QTableView>

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

private:
    Ui::DataSheetForm *ui;

    QSqlQueryModel *m_model;
};

#endif // DATASHEETFORM_H
