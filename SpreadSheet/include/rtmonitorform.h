#ifndef RTMONITORFORM_H
#define RTMONITORFORM_H

#include <QWidget>
#include "singledash.h"
namespace Ui {
class RtMonitorForm;
}

class RtMonitorForm : public QWidget
{
    Q_OBJECT

public:
    explicit RtMonitorForm(QWidget *parent = nullptr);
    ~RtMonitorForm();

private slots:
    void rcvRtData(QJsonObject *, DeviceSymbolInfo);

    void on_pushButton_clicked();

private:
    Ui::RtMonitorForm *ui;
    QList <SingleDash> *m_pListDash;
    int m_nDashCount;


};

#endif // RTMONITORFORM_H
