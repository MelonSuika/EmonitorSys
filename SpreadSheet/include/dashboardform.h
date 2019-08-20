#ifndef DASHBOARDFORM_H
#define DASHBOARDFORM_H

#include <QWidget>
#include <QDebug>
#include <QQuickWidget>

namespace Ui {
class DashBoardForm;
}

class DashBoardForm : public QWidget
{
    Q_OBJECT

public:
    explicit DashBoardForm(QWidget *parent = nullptr);
    ~DashBoardForm();

    void funD(){qDebug()<<"aaa";}
    int m_person;

private slots:
    void on_pushButton_clicked();

    void rcvRtData(int data);

private:
    Ui::DashBoardForm *ui;
    QQuickWidget *m_quickWidget;
};

#endif // DASHBOARDFORM_H
