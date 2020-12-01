#ifndef SINGLEDMJFORM_H
#define SINGLEDMJFORM_H

#include <QWidget>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "common.h"

typedef struct singleDMJData
{
    int nAddress = 0;
    int T = 0;
    int P = 0;
    int D = 0;
    int Ppm = 0;
    int nOP = 0;    // 常压露点
    int nUP = 0;    // 带压露点
    int nType = 0;
    int nDMJType = 0;
}TDMJData;

namespace Ui {
class SingleDMJForm;
}

class SingleDMJForm : public QWidget
{
    Q_OBJECT

public:
    explicit SingleDMJForm(QWidget *parent = nullptr);
    ~SingleDMJForm();
signals:
    void sigRcvDMJType(QString);
private slots:
    void on_pushButton_clicked();

    void on_pushButton_reduce_clicked();

    void on_comboBox_type_currentTextChanged(const QString &arg1);

    void on_comboBox_type_currentIndexChanged(int index);

    void slotRcvRtData(QJsonObject *, DeviceSymbolInfo);
    void slotChangeCombText(QString);

private:
    Ui::SingleDMJForm *ui;
    QGraphicsScene *m_pScene;
    QGraphicsPixmapItem *m_pixItem;
    double m_angle;
    void mousePressEvent(QMouseEvent *event);

    QPointF m_origin;
    QGraphicsLineItem *m_lineH;
    QGraphicsLineItem *m_lineV;
    QGraphicsEllipseItem *m_eclipse;

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

    /* 表盘数据列表 */
    QVector <TDMJData> *m_vctDMJData;
    TDMJData *m_curDMJData;



};

#endif // SINGLEDMJFORM_H
