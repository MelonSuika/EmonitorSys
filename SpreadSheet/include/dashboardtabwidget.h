#ifndef DASHBOARDTABWIDGET_H
#define DASHBOARDTABWIDGET_H
#include <QTabWidget>
#include "rtmonitorform.h"
#include "rtdatasheetform.h"
#include "rtchartform.h"
#include "dashboardform.h"

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
    void sendRtData(QJsonObject *, DeviceSymbolInfo);

private:
    Ui::DashBoardTabWidget *ui;

private slots:
    void rcvRtData(QJsonObject *data, DeviceSymbolInfo deviceSbInfo);
};

#endif // DASHBOARDTABWIDGET_H
