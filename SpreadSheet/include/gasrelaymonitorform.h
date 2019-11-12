#ifndef GASRELAYMONITORFORM_H
#define GASRELAYMONITORFORM_H

#include <QWidget>
#include <QQuickWidget>

namespace Ui {
class GasRelayMonitorForm;
}

class GasRelayMonitorForm : public QWidget
{
    Q_OBJECT

public:
    explicit GasRelayMonitorForm(QWidget *parent = nullptr);
    ~GasRelayMonitorForm();

private:
    Ui::GasRelayMonitorForm *ui;
    QQuickWidget *m_quickWidget;
};

#endif // GASRELAYMONITORFORM_H
