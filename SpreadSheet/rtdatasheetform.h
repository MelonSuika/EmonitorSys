#ifndef RTDATASHEETFORM_H
#define RTDATASHEETFORM_H

#include <QWidget>

namespace Ui {
class RtDataSheetForm;
}

class RtDataSheetForm : public QWidget
{
    Q_OBJECT

public:
    explicit RtDataSheetForm(QWidget *parent = nullptr);
    ~RtDataSheetForm();

private:
    Ui::RtDataSheetForm *ui;
};

#endif // RTDATASHEETFORM_H
