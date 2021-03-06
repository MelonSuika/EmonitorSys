#ifndef DASHBOARDFORM_H
#define DASHBOARDFORM_H

#include <QWidget>
#include <QDebug>
#include <QQuickWidget>
#include "common.h"
#include "rtchartform.h"

namespace Ui {
class DashBoardForm;
}

class DashBoardForm : public QWidget
{
    Q_OBJECT

public:
    explicit DashBoardForm(QWidget *parent = nullptr);
    ~DashBoardForm();

    int m_nPressure;
    int m_nDensity;

signals:
    void sendToMChart(QJsonObject *, DeviceSymbolInfo);

private slots:
    void rcvRtData(QJsonObject *data, DeviceSymbolInfo deviceSbInfo);

private:
    Ui::DashBoardForm *ui;
    QQuickWidget *m_quickWidget;
    RtChartForm *m_chartForm;
};

#endif // DASHBOARDFORM_H
