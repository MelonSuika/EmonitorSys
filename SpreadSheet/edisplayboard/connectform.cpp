#include "connectform.h"
#include "ui_connectform.h"
#include <QDebug>
#include <QStyleOption>
#include <QPainter>
#include <QFileInfo>
#include <QFocusEvent>


ConnectForm::ConnectForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConnectForm)
{
    ui->setupUi(this);

    m_pScene = new QGraphicsScene();
    ui->graphicsView->setScene(m_pScene);
    /* 普通100PR */
    m_pixItem100PRX = new MyGraphicsPixmapItem("100PR");
    m_pixmap100PRX = new QPixmap("./epMaterial/100PR-X.png");
    *m_pixmap100PRX = m_pixmap100PRX->scaled(86, 86, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    qDebug()<<"是否是位图"<<m_pixmap100PRX->isQBitmap();
    m_pixItem100PRX->setPixmap(*m_pixmap100PRX);

    //m_pixItem100PRX->setFlag(QGraphicsItem::ItemIsMovable);
    m_pScene->addItem(m_pixItem100PRX);

    /* 60PR */
    m_pixItem60PRX = new MyGraphicsPixmapItem("60PR");
    m_pixmap60PRX = new QPixmap("./epMaterial/60PR-X.png");
    *m_pixmap60PRX = m_pixmap60PRX->scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItem60PRX->setPixmap(*m_pixmap60PRX);

    m_pScene->addItem(m_pixItem60PRX);

    /* HM100PR */
    m_pixItemHM100PRX = new MyGraphicsPixmapItem("HM100PR");
    m_pixmapHM100PRX = new QPixmap("./epMaterial/HM100PR-X.png");
    *m_pixmapHM100PRX = m_pixmapHM100PRX->scaled(80, 80, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItemHM100PRX->setPixmap(*m_pixmapHM100PRX);

    m_pScene->addItem(m_pixItemHM100PRX);
#if 0
    /* DT */
    m_pixItemDTX = new MyGraphicsPixmapItem("DT");
    m_pixmapDTX = new QPixmap("./epMaterial/DT-X.png");
    *m_pixmapDTX = m_pixmapDTX->scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItemDTX->setPixmap(*m_pixmapDTX);

    m_pScene->addItem(m_pixItemDTX);
#endif
    /* RDH */
    m_pixItemDTX = new MyGraphicsPixmapItem("RDH");
    m_pixmapDTX = new QPixmap("./epMaterial/RDH.png");
    *m_pixmapDTX = m_pixmapDTX->scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItemDTX->setPixmap(*m_pixmapDTX);

    m_pScene->addItem(m_pixItemDTX);
    /* 485 */
    m_pixItem485 = new MyGraphicsPixmapItem;
    m_pixmap485 = new QPixmap("./epMaterial/485.png");
    *m_pixmap485 = m_pixmap485->scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItem485->setPixmap(*m_pixmap485);

    m_pScene->addItem(m_pixItem485);

    /* PC */
    m_pixItemPC = new MyGraphicsPixmapItem;
    m_pixmapPC = new QPixmap("./epMaterial/pc.png");
    *m_pixmapPC = m_pixmapPC->scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItemPC->setPixmap(*m_pixmapPC);

    m_pScene->addItem(m_pixItemPC);

    /* 数传电台 */
    m_pixItemStation = new MyGraphicsPixmapItem;
    m_pixmapStation = new QPixmap("./epMaterial/station.png");
    *m_pixmapStation = m_pixmapStation->scaled(85, 85, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItemStation->setPixmap(*m_pixmapStation);

    m_pScene->addItem(m_pixItemStation);

    /* lora1 */
    m_pixItem100PRWX = new MyGraphicsPixmapItem("100PRW_1");
    m_pixmap100PRWX = new QPixmap("./epMaterial/100PRW-X.png");
    *m_pixmap100PRWX = m_pixmap100PRWX->scaled(78, 78, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItem100PRWX->setPixmap(*m_pixmap100PRWX);

    m_pScene->addItem(m_pixItem100PRWX);

    /* lora2 */
    m_pixItem100PRWX2 = new MyGraphicsPixmapItem("100PRW_2");
    m_pixmap100PRWX2 = new QPixmap("./epMaterial/100PRW-X.png");
    *m_pixmap100PRWX2 = m_pixmap100PRWX2->scaled(78, 78, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItem100PRWX2->setPixmap(*m_pixmap100PRWX2);

    m_pScene->addItem(m_pixItem100PRWX2);

    m_pixItemWireless_1 = new MyGraphicsPixmapItem;
    m_pixmapWireless_1 = new QPixmap("./epMaterial/wireless.png");
    *m_pixmapWireless_1 = m_pixmapWireless_1->scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItemWireless_1->setPixmap(*m_pixmapWireless_1);

    m_pScene->addItem(m_pixItemWireless_1);

    m_pixItemWireless_2 = new MyGraphicsPixmapItem;
    m_pixmapWireless_2 = new QPixmap("./epMaterial/wireless.png");
    *m_pixmapWireless_2 = m_pixmapWireless_2->scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItemWireless_2->setPixmap(*m_pixmapWireless_2);

    m_pScene->addItem(m_pixItemWireless_2);

    m_pixItemWireless_3 = new MyGraphicsPixmapItem;
    m_pixmapWireless_3 = new QPixmap("./epMaterial/wireless.png");
    *m_pixmapWireless_3 = m_pixmapWireless_3->scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    m_pixItemWireless_3->setPixmap(*m_pixmapWireless_3);

    m_pScene->addItem(m_pixItemWireless_3);

    /* 连接线 */
    /* 横1 */
    m_lineItem1 = new QGraphicsLineItem();
    m_lineItem1->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem1->setLine(-100, -20, 230, -20);
    m_pScene->addItem(m_lineItem1);

    /* 竖1 */
    m_lineItem2 = new QGraphicsLineItem();
    m_lineItem2->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem2->setLine(85, -90, 85, -20);
    m_pScene->addItem(m_lineItem2);

    /* 竖2 */
    /*m_lineItem3 = new QGraphicsLineItem();
    m_lineItem3->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem3->setLine(265, 10, 265, 60);
    m_pScene->addItem(m_lineItem3);*/

    m_lineItem32 = new QGraphicsLineItem();
    m_lineItem32->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem32->setLine(-128, 10, -128, 60);
    m_pScene->addItem(m_lineItem32);

    /* 横2 */
    m_lineItem4 = new QGraphicsLineItem();
    m_lineItem4->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem4->setLine(-265, 60, 35, 60);
    m_pScene->addItem(m_lineItem4);

    /*m_lineItem4_2 = new QGraphicsLineItem();
    m_lineItem4_2->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem4_2->setLine(190, 60, 340, 60);
    m_pScene->addItem(m_lineItem4_2);*/

#define END 8
    /* 竖3组 */
    m_lineItem3_1 = new QGraphicsLineItem();
    m_lineItem3_1->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem3_1->setLine(-265, 60, -265, 100 + END);
    m_pScene->addItem(m_lineItem3_1);

    m_lineItem3_2 = new QGraphicsLineItem();
    m_lineItem3_2->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem3_2->setLine(-170, 60, -170, 100 + END);
    m_pScene->addItem(m_lineItem3_2);


    m_lineItem3_3 = new QGraphicsLineItem();
    m_lineItem3_3->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem3_3->setLine(-65, 60, -65, 100 + END);
    m_pScene->addItem(m_lineItem3_3);

    m_lineItem3_4 = new QGraphicsLineItem();
    m_lineItem3_4->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem3_4->setLine(35, 60, 35, 100 + END);
    m_pScene->addItem(m_lineItem3_4);


#if 0
    m_lineItem3_5 = new QGraphicsLineItem();
    m_lineItem3_5->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem3_5->setLine(190, 60, 190, 100 + END);
    m_pScene->addItem(m_lineItem3_5);

    m_lineItem3_6 = new QGraphicsLineItem();
    m_lineItem3_6->setPen(QPen(QColor(255, 255, 255)));
    m_lineItem3_6->setLine(340, 60, 340, 100 + END);
    m_pScene->addItem(m_lineItem3_6);
#endif

#if 0
    /* 添加矩形 */
    QGraphicsRectItem *m_rectItem = new QGraphicsRectItem;
    m_rectItem->setRect(100, 100, 100, 100);
    m_pScene->addItem(m_rectItem);
#endif


    /* position */
    m_pixItem100PRX->setPos(-100, 112);

    m_pixItem60PRX->setPos(-300, 120);
   // m_pixItem60PRX->setFlag(QGraphicsItem::ItemIsMovable);

    m_pixItemHM100PRX->setPos(-200, 124);
    //m_pixItemHM100PRX->setFlag(QGraphicsItem::ItemIsMovable);

    m_pixItemDTX->setPos(21, 123);
    //m_pixItemDTX->setFlag(QGraphicsItem::ItemIsMovable);

    m_pixItem485->setPos(-150, -55);
    //m_pixItem485->setFlag(QGraphicsItem::ItemIsMovable);

    m_pixItemPC->setPos(30, -180);
    //m_pixItemPC->setFlag(QGraphicsItem::ItemIsMovable);

    m_pixItemStation->setPos(225, -70);
    //m_pixItemStation->setFlag(QGraphicsItem::ItemIsMovable);

    m_pixItem100PRWX->setPos(160, 110);
    //m_pixItem100PRWX->setFlag(QGraphicsItem::ItemIsMovable);

    m_pixItem100PRWX2->setPos(310, 110);
    //m_pixItem100PRWX2->setFlag(QGraphicsItem::ItemIsMovable);
    m_pixItemWireless_1->setPos(225+55, -70-15);
    m_pixItemWireless_2->setPos(160+55, 110-15);
    m_pixItemWireless_3->setPos(310+55, 110-15);

#if 0
    QFileInfo fileInfo("../epMaterial/100PR-x.png");
    if(fileInfo.isFile())
    {
        qDebug()<<"存在的";
    }
#endif

#if 0
    QPen pen;   // 定义一个画笔，设置画笔颜色和宽度
    pen.setColor(QColor(0, 160, 230));
    pen.setWidth(10);

    QGraphicsRectItem *m_rectItem = new QGraphicsRectItem();   // 定义一个矩形图元
    m_rectItem->setRect(0, 0, 80, 80);
    m_rectItem->setPen(pen);
    m_rectItem->setBrush(QBrush(QColor(255, 0, 255)));
    m_rectItem->setFlag(QGraphicsItem::ItemIsMovable);


    m_pScene->addItem(m_rectItem);
#endif



    m_pScene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
    ui->graphicsView->show();

    connect(m_pixItem60PRX, SIGNAL(sigSendDMJType(QString)),
            this, SIGNAL(sigDisplayChange(QString)));
    connect(m_pixItem100PRX, SIGNAL(sigSendDMJType(QString)),
            this, SIGNAL(sigDisplayChange(QString)));
    connect(m_pixItemHM100PRX, SIGNAL(sigSendDMJType(QString)),
            this, SIGNAL(sigDisplayChange(QString)));
    connect(m_pixItemDTX, SIGNAL(sigSendDMJType(QString)),
            this, SIGNAL(sigDisplayChange(QString)));
    connect(m_pixItem100PRWX, SIGNAL(sigSendDMJType(QString)),
            this, SIGNAL(sigDisplayChange(QString)));
    connect(m_pixItem100PRWX2, SIGNAL(sigSendDMJType(QString)),
            this, SIGNAL(sigDisplayChange(QString)));


}

ConnectForm::~ConnectForm()
{
    delete ui;
}

void ConnectForm::on_pushButton_clicked()
{
    qDebug()<<m_pScene->width()<<m_pScene->height()<<"view:"
           <<ui->graphicsView->x()<<ui->graphicsView->y()<<ui->graphicsView->width()<<ui->graphicsView->height();
}
