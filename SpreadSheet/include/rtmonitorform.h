#ifndef RTMONITORFORM_H
#define RTMONITORFORM_H

#include <QWidget>
#include "singledash.h"
#include "pagenavigator.h"
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

    void upDatePageWidget(int page);

    void on_pushButton_removeDash_clicked();

    void on_pushButton_addDash_clicked();

private:
    Ui::RtMonitorForm *ui;
    QList <SingleDashForm *> *m_pListDash;
    int m_nTestDashCount;
    PageNavigator *m_pPageNavigator;


};

#endif // RTMONITORFORM_H
