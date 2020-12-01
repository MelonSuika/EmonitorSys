#ifndef MULDMJFORM_H
#define MULDMJFORM_H

#include <QWidget>
#include <QGraphicsLineItem>
#include "common.h"
namespace Ui {
class MulDMJForm;
}

class MulDMJForm : public QWidget
{
    Q_OBJECT

public:
    explicit MulDMJForm(QWidget *parent = nullptr);
    ~MulDMJForm();

private:
    Ui::MulDMJForm *ui;
    /* 表盘指针相关 */
    QGraphicsLineItem *m_curPointer;
    QGraphicsLineItem *m_pointer60PR;
    QGraphicsLineItem *m_pointer100PR;
    QGraphicsLineItem *m_pointerHM100PR;
    QGraphicsLineItem *m_pointer100PRW_1;
    QGraphicsLineItem *m_pointer100PRW_2;

    /* RDH无表盘，用文字显示 */
    QGraphicsSimpleTextItem *m_txtItemT;
    QGraphicsSimpleTextItem *m_txtItemP;
    QGraphicsSimpleTextItem *m_txtItemD;
    QGraphicsSimpleTextItem *m_txtItemPpm;
    QGraphicsSimpleTextItem *m_txtItemOP;
    QGraphicsSimpleTextItem *m_txtItemUP;

    /* 表盘背景图相关 */
    QPixmap *m_pixMap; // 当前背景
    QPixmap *m_pixMap60PR;
    QPixmap *m_pixMap100PR;
    QPixmap *m_pixMapHM100PR;
    QPixmap *m_pixMap100PRW_1;
    QPixmap *m_pixMap100PRW_2;
    QPixmap *m_pixMapDT;

    /* 表盘图元相关 */
    QGraphicsPixmapItem *m_pixItem; // 当前背景
    QGraphicsPixmapItem *m_pixItem60PR;
    QGraphicsPixmapItem *m_pixItem100PR;
    QGraphicsPixmapItem *m_pixItemHM100PR;
    QGraphicsPixmapItem *m_pixItem100PRW_1;
    QGraphicsPixmapItem *m_pixItem100PRW_2;
    QGraphicsPixmapItem *m_pixItemDT;

    /* 各区场景 */
    QGraphicsScene *m_pScene60PR;
    QGraphicsScene *m_pScene100PR;
    QGraphicsScene *m_pSceneHM100PR;
    QGraphicsScene *m_pSceneDT;
    QGraphicsScene *m_pScene100PRW1;
    QGraphicsScene *m_pScene100PRW2;

private slots:
    void slotRcvRtData(QJsonObject *, DeviceSymbolInfo);
};

#endif // MULDMJFORM_H
