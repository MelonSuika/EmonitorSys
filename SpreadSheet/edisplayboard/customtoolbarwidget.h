#ifndef CUSTOMTOOLBARWIDGET_H
#define CUSTOMTOOLBARWIDGET_H

#include <QWidget>

namespace Ui {
class CustomToolBarWidget;
}

class CustomToolBarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CustomToolBarWidget(QWidget *parent = nullptr);
    ~CustomToolBarWidget();

private:
    Ui::CustomToolBarWidget *ui;
};

#endif // CUSTOMTOOLBARWIDGET_H
