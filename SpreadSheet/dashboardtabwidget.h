#ifndef DASHBOARDTABWIDGET_H
#define DASHBOARDTABWIDGET_H

#include <QTabWidget>

namespace Ui {
class DashBoardTabWidget;
}

class DashBoardTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit DashBoardTabWidget(QWidget *parent = nullptr);
    ~DashBoardTabWidget();

signals:
    void sendRtData(int);

private:
    Ui::DashBoardTabWidget *ui;

private slots:
    void rcvRtData(int n);
};

#endif // DASHBOARDTABWIDGET_H
