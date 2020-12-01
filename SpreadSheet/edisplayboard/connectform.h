#ifndef CONNECTFORM_H
#define CONNECTFORM_H

#include <QWidget>
#include <QGraphicsItem>
#include "mygraphicspixmapitem.h"
#include "common.h"

namespace Ui {
class ConnectForm;
}

class ConnectForm : public QWidget
{
    Q_OBJECT

public:
    explicit ConnectForm(QWidget *parent = nullptr);
    ~ConnectForm();

private slots:
    void on_pushButton_clicked();


signals:
    void sigDisplayChange(QString);
private:
    Ui::ConnectForm *ui;
    QGraphicsScene *m_pScene;

    /* 拓扑图图元 */
    MyGraphicsPixmapItem *m_pixItem100PRX;
    MyGraphicsPixmapItem *m_pixItem60PRX;
    MyGraphicsPixmapItem *m_pixItemHM100PRX;
    MyGraphicsPixmapItem *m_pixItemDTX;
    MyGraphicsPixmapItem *m_pixItem100PRWX;
    MyGraphicsPixmapItem *m_pixItem100PRWX2;
    MyGraphicsPixmapItem *m_pixItemPC;
    MyGraphicsPixmapItem *m_pixItem485;
    MyGraphicsPixmapItem *m_pixItemStation;
    MyGraphicsPixmapItem *m_pixItemWireless_1;
    MyGraphicsPixmapItem *m_pixItemWireless_2;
    MyGraphicsPixmapItem *m_pixItemWireless_3;

    /* 连接线 */
    QGraphicsLineItem *m_lineItem1;
    QGraphicsLineItem *m_lineItem2;
    QGraphicsLineItem *m_lineItem3;
    QGraphicsLineItem *m_lineItem32;
    QGraphicsLineItem *m_lineItem4;
    QGraphicsLineItem *m_lineItem4_2;
    QGraphicsLineItem *m_lineItem5;

    QGraphicsLineItem *m_lineItem3_1;
    QGraphicsLineItem *m_lineItem3_2;
    QGraphicsLineItem *m_lineItem3_3;
    QGraphicsLineItem *m_lineItem3_4;
    QGraphicsLineItem *m_lineItem3_5;
    QGraphicsLineItem *m_lineItem3_6;

    /* 图元背景 */
    QPixmap *m_pixmap100PRX;
    QPixmap *m_pixmap60PRX;
    QPixmap *m_pixmapHM100PRX;
    QPixmap *m_pixmapDTX;
    QPixmap *m_pixmap100PRWX;
    QPixmap *m_pixmap100PRWX2;
    QPixmap *m_pixmapPC;
    QPixmap *m_pixmap485;
    QPixmap *m_pixmapStation;
    QPixmap *m_pixmapWireless_1;
    QPixmap *m_pixmapWireless_2;
    QPixmap *m_pixmapWireless_3;

};

#endif // CONNECTFORM_H
