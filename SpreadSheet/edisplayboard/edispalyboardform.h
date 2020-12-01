#ifndef EDISPALYBOARDFORM_H
#define EDISPALYBOARDFORM_H

#include <QWidget>
#include "common.h"
namespace Ui {
class EDispalyBoardForm;
}

class EDispalyBoardForm : public QWidget
{
    Q_OBJECT

public:
    explicit EDispalyBoardForm(QWidget *parent = nullptr);
    ~EDispalyBoardForm();

private:
    Ui::EDispalyBoardForm *ui;


signals:
    void sigSend2SubForm(QJsonObject *, DeviceSymbolInfo);
private slots:
    void slotRcvMainEp(QJsonObject *, DeviceSymbolInfo);
};

#endif // EDISPALYBOARDFORM_H
