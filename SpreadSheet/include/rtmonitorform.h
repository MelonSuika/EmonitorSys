#ifndef RTMONITORFORM_H
#define RTMONITORFORM_H

#include <QWidget>

namespace Ui {
class RtMonitorForm;
}

class RtMonitorForm : public QWidget
{
    Q_OBJECT

public:
    explicit RtMonitorForm(QWidget *parent = nullptr);
    ~RtMonitorForm();

private:
    Ui::RtMonitorForm *ui;
};

#endif // RTMONITORFORM_H
