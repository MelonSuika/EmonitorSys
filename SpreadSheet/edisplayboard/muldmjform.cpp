#include "muldmjform.h"
#include "ui_muldmjform.h"

MulDMJForm::MulDMJForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MulDMJForm)
{
    ui->setupUi(this);

    /* 表盘部分 */
    /* 60PR */
    m_pixMap60PR = new QPixmap("./epMaterial/60PR-Z-G.png");
    *m_pixMap60PR = m_pixMap60PR->scaled(350*0.6, 350*0.6, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItem60PR = new QGraphicsPixmapItem();
    m_pixItem60PR->setPixmap(*m_pixMap60PR);
    m_pScene60PR = new QGraphicsScene();
    m_pScene60PR->addItem(m_pixItem60PR);
    m_pixItem60PR->setPos(m_pScene60PR->width()/2-m_pixMap60PR->width()/2, m_pScene60PR->height()/2-m_pixMap60PR->height()/2);
    ui->graphicsView_60PR->setScene(m_pScene60PR);
    ui->graphicsView_60PR->show();


    /* DT */
    m_pixMapDT = new QPixmap("./epMaterial/RDH.png");
    *m_pixMapDT = m_pixMapDT->scaled(300*0.6, 300*0.6, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItemDT = new QGraphicsPixmapItem();
    m_pixItemDT->setPixmap(*m_pixMapDT);
    m_pSceneDT = new QGraphicsScene();
    m_pSceneDT->addItem(m_pixItemDT);
    m_pixItemDT->setPos(m_pSceneDT->width()/2-m_pixMapDT->width()/2+93, m_pSceneDT->height()/2-m_pixMapDT->height()/2);
    ui->graphicsView_DT->setScene(m_pSceneDT);
    ui->graphicsView_DT->show();

    /* 100PR */
    m_pixMap100PR = new QPixmap("./epMaterial/100PR-Z.png");
    *m_pixMap100PR = m_pixMap100PR->scaled(350*0.6, 350*0.6, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItem100PR = new QGraphicsPixmapItem();
    m_pixItem100PR->setPixmap(*m_pixMap100PR);
    m_pScene100PR = new QGraphicsScene();
    m_pScene100PR->addItem(m_pixItem100PR);
    m_pixItem100PR->setPos(m_pScene100PR->width()/2-m_pixMap100PR->width()/2, m_pScene100PR->height()/2-m_pixMap100PR->height()/2);
    ui->graphicsView_100PR->setScene(m_pScene100PR);
    ui->graphicsView_100PR->show();

    /* HM100PR */
    m_pixMapHM100PR = new QPixmap("./epMaterial/HM100PR-Z.png");
    *m_pixMapHM100PR = m_pixMapHM100PR->scaled(350*0.6, 350*0.6, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItemHM100PR = new QGraphicsPixmapItem();
    m_pixItemHM100PR->setPixmap(*m_pixMapHM100PR);
    m_pSceneHM100PR = new QGraphicsScene();
    m_pSceneHM100PR->addItem(m_pixItemHM100PR);
    m_pixItemHM100PR->setPos(m_pSceneHM100PR->width()/2-m_pixMapHM100PR->width()/2,
                             m_pSceneHM100PR->height()/2-m_pixMapHM100PR->height()/2);
    ui->graphicsView_HM100PR->setScene(m_pSceneHM100PR);
    ui->graphicsView_HM100PR->show();


    /* 100PRW_1 */
    m_pixMap100PRW_1 = new QPixmap("./epMaterial/100PRW-Z.png");
    *m_pixMap100PRW_1 = m_pixMap100PRW_1->scaled(400*0.56, 400*0.56, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItem100PRW_1 = new QGraphicsPixmapItem();
    m_pixItem100PRW_1->setPixmap(*m_pixMap100PRW_1);
    m_pScene100PRW1 = new QGraphicsScene();
    m_pScene100PRW1->addItem(m_pixItem100PRW_1);
    m_pixItem100PRW_1->setPos(m_pScene100PRW1->width()/2-m_pixMap100PRW_1->width()/2,
                             m_pScene100PRW1->height()/2-m_pixMap100PRW_1->height()/2);
    ui->graphicsView_100PRW1->setScene(m_pScene100PRW1);
    ui->graphicsView_100PRW1->show();


    /* 100PRW_2 */
    m_pixMap100PRW_2 = new QPixmap("./epMaterial/100PRW-Z.png");
    *m_pixMap100PRW_2 = m_pixMap100PRW_2->scaled(400*0.56, 400*0.56, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItem100PRW_2 = new QGraphicsPixmapItem();
    m_pixItem100PRW_2->setPixmap(*m_pixMap100PRW_2);
    m_pScene100PRW2 = new QGraphicsScene();
    m_pScene100PRW2->addItem(m_pixItem100PRW_2);
    m_pixItem100PRW_2->setPos(m_pScene100PRW2->width()/2-m_pixMap100PRW_2->width()/2,
                             m_pScene100PRW2->height()/2-m_pixMap100PRW_2->height()/2);
    ui->graphicsView_100PRW2->setScene(m_pScene100PRW2);
    ui->graphicsView_100PRW2->show();


    /* 指针部分 */
#define RDHSTR ""
    /* RDH */
    m_txtItemT = new QGraphicsSimpleTextItem();
    m_txtItemT->setBrush(QBrush(Qt::white));
    m_txtItemT->setFont(QFont("微软雅黑", 8));
    m_txtItemT->setText(RDHSTR);
    m_txtItemT->moveBy(m_pSceneDT->width()/2 - m_pixMapDT->width()/2 + 155, m_pSceneDT->height()/2-m_pixMapDT->height()/2 - 10);
    m_pSceneDT->addItem(m_txtItemT);


    m_txtItemP = new QGraphicsSimpleTextItem();
    m_txtItemP->setBrush(QBrush(Qt::white));
    m_txtItemP->setFont(QFont("微软雅黑", 8));
    m_txtItemP->setText(RDHSTR);
    m_txtItemP->moveBy(m_pSceneDT->width()/2 - m_pixMapDT->width()/2 + 155, m_pSceneDT->height()/2-m_pixMapDT->height()/2 + 10 );
    m_pSceneDT->addItem(m_txtItemP);


    m_txtItemD = new QGraphicsSimpleTextItem();
    m_txtItemD->setBrush(QBrush(Qt::white));
    m_txtItemD->setFont(QFont("微软雅黑", 8));
    m_txtItemD->setText(RDHSTR);
    m_txtItemD->moveBy(m_pSceneDT->width()/2 - m_pixMapDT->width()/2 + 155, m_pSceneDT->height()/2-m_pixMapDT->height()/2 + 30);
    m_pSceneDT->addItem(m_txtItemD);


    m_txtItemPpm = new QGraphicsSimpleTextItem();
    m_txtItemPpm->setBrush(QBrush(Qt::white));
    m_txtItemPpm->setFont(QFont("微软雅黑", 8));
    m_txtItemPpm->setText(RDHSTR);
    m_txtItemPpm->moveBy(m_pSceneDT->width()/2 - m_pixMapDT->width()/2 + 155, m_pSceneDT->height()/2-m_pixMapDT->height()/2 + 50);
    m_pSceneDT->addItem(m_txtItemPpm);


    m_txtItemOP = new QGraphicsSimpleTextItem();
    m_txtItemOP->setBrush(QBrush(Qt::white));
    m_txtItemOP->setFont(QFont("微软雅黑", 8));
    m_txtItemOP->setText(RDHSTR);
    m_txtItemOP->moveBy(m_pSceneDT->width()/2 - m_pixMapDT->width()/2 + 155, m_pSceneDT->height()/2-m_pixMapDT->height()/2 + 70);
    m_pSceneDT->addItem(m_txtItemOP);


    m_txtItemUP = new QGraphicsSimpleTextItem();
    m_txtItemUP->setBrush(QBrush(Qt::white));
    m_txtItemUP->setFont(QFont("微软雅黑", 8));
    m_txtItemUP->setText(RDHSTR);
    m_txtItemUP->moveBy(m_pSceneDT->width()/2 - m_pixMapDT->width()/2 + 155, m_pSceneDT->height()/2-m_pixMapDT->height()/2 + 90);
    m_pSceneDT->addItem(m_txtItemUP);

    /* 60PR指针 */
    m_pointer60PR = new QGraphicsLineItem();
    QPen pen(QColor(205, 0, 0));
    pen.setWidth(3);
    m_pointer60PR->setPen(pen);
    m_pointer60PR->setLine(0, 0, 0, 30*0.6);
    m_pointer60PR->moveBy(m_pScene60PR->width()/2 - m_pixMap60PR->width()/2 + 106*0.6, m_pScene60PR->height()/2-m_pixMap60PR->height()/2 + 202*0.6);
    m_pScene60PR->addItem(m_pointer60PR);
    m_pointer60PR->setTransformOriginPoint(0, -45*0.6);
    m_pointer60PR->setRotation(100/200.0*270+45);

    /* 100PR指针 */
    m_pointer100PR = new QGraphicsLineItem();
    QPen pen100PR(QColor(205, 0, 0));
    pen100PR.setWidth(3);
    m_pointer100PR->setPen(pen100PR);
    m_pointer100PR->setLine(0, 0, 0, 50*0.6);
    m_pointer100PR->moveBy(m_pScene100PR->width()/2 - m_pixMap100PR->width()/2 + 129*0.6, m_pScene100PR->height()/2-m_pixMap100PR->height()/2 + 171*0.6);
    m_pScene100PR->addItem(m_pointer100PR);
    m_pointer100PR->setTransformOriginPoint(0, -45*0.6);
    m_pointer100PR->setRotation(100/1000.0*270+45);

    /* HM100PR指针 */
    m_pointerHM100PR = new QGraphicsLineItem();
    QPen penHM100PR(QColor(205, 0, 0));
    penHM100PR.setWidth(3);
    m_pointerHM100PR->setPen(penHM100PR);
    m_pointerHM100PR->setLine(0, 0, 0, 50*0.6);
    m_pointerHM100PR->moveBy(m_pSceneHM100PR->width()/2 - m_pixMapHM100PR->width()/2 + 125*0.6, m_pSceneHM100PR->height()/2-m_pixMapHM100PR->height()/2 + 165*0.6);
    m_pSceneHM100PR->addItem(m_pointerHM100PR);
    m_pointerHM100PR->setTransformOriginPoint(0, -42*0.6);
    m_pointerHM100PR->setRotation(72);

    /* 100PRW_1指针 */
    m_pointer100PRW_1 = new QGraphicsLineItem();
    QPen pen100PRW_1(QColor(205, 0, 0));
    pen100PRW_1.setWidth(3);
    m_pointer100PRW_1->setPen(pen100PRW_1);
    m_pointer100PRW_1->setLine(0, 0, 0, 50*0.53);
    m_pointer100PRW_1->moveBy(m_pScene100PRW1->width()/2 - m_pixMap100PRW_1->width()/2 + 167*0.53+4, m_pScene100PRW1->height()/2-m_pixMap100PRW_1->height()/2 + 240*0.53+4);
    m_pScene100PRW1->addItem(m_pointer100PRW_1);
    m_pointer100PRW_1->setTransformOriginPoint(0, -48*0.53+1);
    m_pointer100PRW_1->setRotation(72);

    // 167 240 -48
    //
    /* 100PRW_2指针 */
    m_pointer100PRW_2 = new QGraphicsLineItem();
    QPen pen100PRW_2(QColor(205, 0, 0));
    pen100PRW_2.setWidth(3);
    m_pointer100PRW_2->setPen(pen100PRW_2);
    m_pointer100PRW_2->setLine(0, 0, 0, 50*0.53);
    m_pointer100PRW_2->moveBy(m_pScene100PRW2->width()/2 - m_pixMap100PRW_2->width()/2 + 167*0.53+4, m_pScene100PRW2->height()/2-m_pixMap100PRW_2->height()/2 + 240*0.53+4);
    m_pScene100PRW2->addItem(m_pointer100PRW_2);
    m_pointer100PRW_2->setTransformOriginPoint(0, -48*0.53+1);
    m_pointer100PRW_2->setRotation(72);

}

MulDMJForm::~MulDMJForm()
{
    delete ui;
}

void MulDMJForm::slotRcvRtData(QJsonObject *data, DeviceSymbolInfo info)
{
    int nDMJType = info.nDMJType;
    int t = data->value("温度").toInt();
    short n16T = (short)t;
    int p = data->value("压力").toInt();
    int d = data->value("密度").toInt();
    int nPpm = data->value("Ppm").toInt();
    int address = data->value("地址").toInt();
    int nUP = data->value("带压露点").toInt();
    short n16UP = (short)nUP;
    int nOP = data->value("常压露点").toInt();
    short n16OP = (short)nOP;
    if (nDMJType == DMJ60PR_1)
    {
        m_pointer60PR->setRotation(d/2000.0*270 + 45);
    }
    else if (nDMJType == DMJ100PR_1)
    {
        m_pointer100PR->setRotation(d/10000.0*270 + 45);
    }
    else if(nDMJType == DMJ100PRW_1)
    {
        m_pointer100PRW_1->setRotation(d/10000.0*270 + 45);
        m_pointer100PRW_2->setRotation(d/10000.0*270 + 45);

    }
    else if(nDMJType == DMJHM100PR_1)
    {
        m_pointerHM100PR->setRotation(d/10000.0*270 + 45);
    }
    else if (nDMJType == DMJ_RDH_1)
    {
        m_txtItemT->setText("温度:" + QString::asprintf("%.2f", (float)n16T/100) + "℃");
        m_txtItemD->setText("密度:" + QString::asprintf("%.3f", (float)d/10000) + "MPa");
        m_txtItemP->setText("压力:" + QString::asprintf("%.3f", (float)p/10000) + "MPa");
        m_txtItemPpm->setText("微水:" + QString::number(nPpm) + "ppm");
        m_txtItemOP->setText("常压露点:" + QString::asprintf("%.2f", (float)n16OP/100) + "℃");
        m_txtItemUP->setText("带压露点:" + QString::asprintf("%.2f", (float)n16UP/100) + "℃");
    }

}
