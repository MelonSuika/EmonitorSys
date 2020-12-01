#ifndef CURVEFORM_H
#define CURVEFORM_H

#include <QWidget>
#include "common.h"

namespace Ui {
class CurveForm;
}

class CurveForm : public QWidget
{
    Q_OBJECT

public:
    explicit CurveForm(QWidget *parent = nullptr);
    ~CurveForm();
signals:
    void sigSend2Chart(QJsonObject *, DeviceSymbolInfo);
    void sigPassThrough(QString);

private:
    Ui::CurveForm *ui;
private slots:
    void slotRcvRtData(QJsonObject *, DeviceSymbolInfo);

};

#endif // CURVEFORM_H
