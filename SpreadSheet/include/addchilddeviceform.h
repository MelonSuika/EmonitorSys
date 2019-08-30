#ifndef ADDCHILDDEVICEFORM_H
#define ADDCHILDDEVICEFORM_H

#include <QWidget>
#include "serialportinfo.h"

namespace Ui {
class AddChildDeviceForm;
}

class AddChildDeviceForm : public QWidget
{
    Q_OBJECT

public:
    explicit AddChildDeviceForm(QWidget *parent = nullptr);
    ~AddChildDeviceForm();

    void Create(QList<SerialPortInfo> *pComlist);

private slots:
    void on_pushButton_ok_clicked();

private:
    Ui::AddChildDeviceForm *ui;
    QList<SerialPortInfo> *m_pComlist;
};

#endif // ADDCHILDDEVICEFORM_H
