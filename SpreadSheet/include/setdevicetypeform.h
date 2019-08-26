#ifndef SETDEVICETYPEFORM_H
#define SETDEVICETYPEFORM_H

#include <QWidget>
#include "serialportinfo.h"

namespace Ui {
class SetDeviceTypeForm;
}

class SetDeviceTypeForm : public QWidget
{
    Q_OBJECT

public:
    explicit SetDeviceTypeForm(QWidget *parent = nullptr);
    ~SetDeviceTypeForm();

    void Create(QList<SerialPortInfo> *pComlist);

private slots:
    void on_pushButton_clicked();

private:
    Ui::SetDeviceTypeForm *ui;
    QList<SerialPortInfo> *m_pComlist;
};

#endif // SETDEVICETYPEFORM_H
