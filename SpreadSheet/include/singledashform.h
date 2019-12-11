#ifndef SINGLEDASHFORM_H
#define SINGLEDASHFORM_H

#include <QWidget>
#include "common.h"
namespace Ui {
class SingleDashForm;
}

class SingleDashForm : public QWidget
{
    Q_OBJECT

public:
    explicit SingleDashForm(QWidget *parent = nullptr);
    ~SingleDashForm();

    void setDashData(QJsonObject *data);

    DeviceSymbolInfo m_deviceSbInfo;
private:
    Ui::SingleDashForm *ui;
    QQuickWidget *m_quickWidget;


};

#endif // SINGLEDASHFORM_H
