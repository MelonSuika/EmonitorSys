#include "singledmjform.h"
#include "ui_singledmjform.h"
#include <QDebug>
#include <QMouseEvent>

SingleDMJForm::SingleDMJForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SingleDMJForm)
{
    ui->setupUi(this);

    /* 初始表盘角度 */
    m_angle = 72;
    m_origin.setX(0);
    m_origin.setY(0);

    m_vctDMJData = new QVector <TDMJData>;
    m_curDMJData = nullptr;

    m_pScene = new QGraphicsScene();
    qDebug()<<"first sence w h"<<m_pScene->width()<<m_pScene->height();
    m_pScene->setSceneRect(0, 0, 685, 475);
    ui->graphicsView_DMJ->setScene(m_pScene);
    ui->graphicsView_DMJ->show();

    // 滚动条关闭，designer里设置

    /* 60PR */
    m_pixMap60PR = new QPixmap("./epMaterial/60PR-Z-G.png");
    *m_pixMap60PR = m_pixMap60PR->scaled(350, 350, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    /* 100PR */
    m_pixMap100PR = new QPixmap("./epMaterial/100PR-Z.png");
    *m_pixMap100PR = m_pixMap100PR->scaled(350, 350, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    /* HM100PR */
    m_pixMapHM100PR = new QPixmap("./epMaterial/HM100PR-Z.png");
    *m_pixMapHM100PR = m_pixMapHM100PR->scaled(350, 350, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    /* 100PRW_1 */
    m_pixMap100PRW_1 = new QPixmap("./epMaterial/100PRW-Z.png");
    *m_pixMap100PRW_1 = m_pixMap100PRW_1->scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    /* 100PRW_2 */
    m_pixMap100PRW_2 = new QPixmap("./epMaterial/100PRW-Z.png");
    *m_pixMap100PRW_2 = m_pixMap100PRW_2->scaled(400, 400, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    /* DT */
    m_pixMapDT = new QPixmap("./epMaterial/RDH.png");
    *m_pixMapDT = m_pixMapDT->scaled(350, 350, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    /* 选择当前pixmap */
    m_pixItem = new QGraphicsPixmapItem;
    m_pixMap = m_pixMap60PR;

    m_pixItem->setPixmap(*m_pixMap);

    m_pScene->addItem(m_pixItem);
    m_pixItem->setPos(m_pScene->width()/2-m_pixMap->width()/2, m_pScene->height()/2-m_pixMap->height()/2);

    /* RDH */
    m_txtItemT = new QGraphicsSimpleTextItem();
    m_txtItemT->setBrush(QBrush(Qt::white));
    m_txtItemT->setFont(QFont("微软雅黑", 12));
    m_txtItemT->moveBy(520, 80);
    m_pScene->addItem(m_txtItemT);

    m_txtItemP = new QGraphicsSimpleTextItem();
    m_txtItemP->setBrush(QBrush(Qt::white));
    m_txtItemP->setFont(QFont("微软雅黑", 12));
    m_txtItemP->moveBy(520, 110);
    m_pScene->addItem(m_txtItemP);

    m_txtItemD = new QGraphicsSimpleTextItem();
    m_txtItemD->setBrush(QBrush(Qt::white));
    m_txtItemD->setFont(QFont("微软雅黑", 12));
    m_txtItemD->moveBy(520, 140);
    m_pScene->addItem(m_txtItemD);

    m_txtItemPpm = new QGraphicsSimpleTextItem();
    m_txtItemPpm->setBrush(QBrush(Qt::white));
    m_txtItemPpm->setFont(QFont("微软雅黑", 12));
    m_txtItemPpm->moveBy(520, 170);
    m_pScene->addItem(m_txtItemPpm);

    m_txtItemOP = new QGraphicsSimpleTextItem();
    m_txtItemOP->setBrush(QBrush(Qt::white));
    m_txtItemOP->setFont(QFont("微软雅黑", 12));
    m_txtItemOP->moveBy(520, 200);
    m_pScene->addItem(m_txtItemOP);

    m_txtItemUP = new QGraphicsSimpleTextItem();
    m_txtItemUP->setBrush(QBrush(Qt::white));
    m_txtItemUP->setFont(QFont("微软雅黑", 12));
    m_txtItemUP->moveBy(520, 230);
    m_pScene->addItem(m_txtItemUP);

    /* 60PR指针 */
    m_pointer60PR = new QGraphicsLineItem();
    QPen pen(QColor(205, 0, 0));
    pen.setWidth(3);
    m_pointer60PR->setPen(pen);
    m_pointer60PR->setLine(0, 0, 0, 30);
    m_pointer60PR->moveBy(m_pScene->width()/2 - m_pixMap60PR->width()/2 + 106, m_pScene->height()/2-m_pixMap60PR->height()/2 + 202);
    m_pScene->addItem(m_pointer60PR);
    m_pointer60PR->setTransformOriginPoint(0, -45);
    m_pointer60PR->setRotation(100/200.0*270+45);
    m_pointer60PR->setVisible(false);

    /* 100PR指针 */
    m_pointer100PR = new QGraphicsLineItem();
    QPen pen100PR(QColor(205, 0, 0));
    pen100PR.setWidth(3);
    m_pointer100PR->setPen(pen100PR);
    m_pointer100PR->setLine(0, 0, 0, 50);
    m_pointer100PR->moveBy(m_pScene->width()/2 - m_pixMap100PR->width()/2 + 129, m_pScene->height()/2-m_pixMap100PR->height()/2 + 171);
    m_pScene->addItem(m_pointer100PR);
    m_pointer100PR->setTransformOriginPoint(0, -45);
    m_pointer100PR->setRotation(m_angle);
    m_pointer100PR->setVisible(false);

    /* HM100PR指针 */
    m_pointerHM100PR = new QGraphicsLineItem();
    QPen penHM100PR(QColor(205, 0, 0));
    penHM100PR.setWidth(3);
    m_pointerHM100PR->setPen(penHM100PR);
    m_pointerHM100PR->setLine(0, 0, 0, 50);
    m_pointerHM100PR->moveBy(m_pScene->width()/2 - m_pixMapHM100PR->width()/2 + 125, m_pScene->height()/2-m_pixMapHM100PR->height()/2 + 165);
    m_pScene->addItem(m_pointerHM100PR);
    m_pointerHM100PR->setTransformOriginPoint(0, -42);
    m_pointerHM100PR->setRotation(m_angle);
    m_pointerHM100PR->setVisible(false);

    /* 100PRW_1指针 */
    m_pointer100PRW_1 = new QGraphicsLineItem();
    QPen pen100PRW_1(QColor(205, 0, 0));
    pen100PRW_1.setWidth(3);
    m_pointer100PRW_1->setPen(pen100PRW_1);
    m_pointer100PRW_1->setLine(0, 0, 0, 50);
    m_pointer100PRW_1->moveBy(m_pScene->width()/2 - m_pixMap100PRW_1->width()/2 + 167, m_pScene->height()/2-m_pixMap100PRW_1->height()/2 + 240);
    m_pScene->addItem(m_pointer100PRW_1);
    m_pointer100PRW_1->setTransformOriginPoint(0, -48);
    m_pointer100PRW_1->setRotation(m_angle);
    m_pointer100PRW_1->setVisible(false);

    /* 100PRW_2指针 */
    m_pointer100PRW_2 = new QGraphicsLineItem();
    QPen pen100PRW_2(QColor(205, 0, 0));
    pen100PRW_2.setWidth(3);
    m_pointer100PRW_2->setPen(pen100PRW_2);
    m_pointer100PRW_2->setLine(0, 0, 0, 50);
    m_pointer100PRW_2->moveBy(m_pScene->width()/2 - m_pixMap100PRW_2->width()/2 + 167, m_pScene->height()/2-m_pixMap100PRW_2->height()/2 + 240);
    m_pScene->addItem(m_pointer100PRW_2);
    m_pointer100PRW_2->setTransformOriginPoint(0, -48);
    m_pointer100PRW_2->setRotation(m_angle);
    m_pointer100PRW_2->setVisible(false);


    /* 选择当前指针 */
    m_curPointer = m_pointer60PR;
    m_curPointer->setVisible(true);

    /* 标注原点的十字 */
    m_lineH = new QGraphicsLineItem();
    m_lineH->setPen(QPen(QColor(50, 200, 102)));
    m_lineH->setLine(m_pScene->width()/2, m_pScene->height()/2 - 20, m_pScene->width()/2, m_pScene->height()/2 + 20);
    m_lineV = new QGraphicsLineItem();
    m_lineV->setPen(QPen(QColor(50, 200, 102)));
    m_lineV->setLine(m_pScene->width()/2 - 20, m_pScene->height()/2, m_pScene->width()/2 + 20, m_pScene->height()/2);
    m_pScene->addItem(m_lineH);
    m_pScene->addItem(m_lineV);

    /* 椭圆 */
    m_eclipse = new QGraphicsEllipseItem();
    QPen pen3(QColor(200, 200, 100));
    pen3.setWidth(2);
    m_eclipse->setPen(pen3);
    m_eclipse->setRect(QRectF(0, 0, 210, 210));
    m_eclipse->setStartAngle(16*90);
    m_eclipse->setSpanAngle(16*270);
    m_eclipse->setFlag(QGraphicsItem::ItemIsMovable);
    m_pScene->addItem(m_eclipse);
    m_eclipse->setVisible(false);
    m_lineH->setVisible(false);
    m_lineV->setVisible(false);

    /* 隐藏部分测试按钮 */
    ui->pushButton->setVisible(false);
    ui->pushButton_reduce->setVisible(false);
    //ui->label->setVisible(false);
    //ui->lineEdit_com->setVisible(false);
    ui->lineEdit_type->setVisible(false);
    ui->label_type->setVisible(false);


    connect(this, SIGNAL(sigRcvDMJType(QString)), this, SLOT(on_comboBox_type_currentTextChanged(QString)));

}

SingleDMJForm::~SingleDMJForm()
{
    delete ui;
}

void SingleDMJForm::on_pushButton_clicked()
{

    m_angle += 45;
    m_curPointer->setRotation(m_angle);
    qDebug()<<"sence w h"<<m_pScene->width()<<m_pScene->height();
}

void SingleDMJForm::on_pushButton_reduce_clicked()
{
    qDebug()<<"pointer"<<m_curPointer->pos();
    qDebug()<<"setTransformOriginPoint"<<m_origin.x()-m_curPointer->x()<<m_origin.y()-m_curPointer->y();
    m_angle -= 5;
    m_curPointer->setRotation(m_angle);
}



void SingleDMJForm::on_comboBox_type_currentTextChanged(const QString &arg1)
{
    m_curPointer->setVisible(false);
    m_txtItemD->setVisible(false);
    m_txtItemP->setVisible(false);
    m_txtItemT->setVisible(false);
    m_txtItemPpm->setVisible(false);
    m_txtItemOP->setVisible(false);
    m_txtItemUP->setVisible(false);
    if (arg1 == "60PR")
    {
        m_pixMap = m_pixMap60PR;
        m_pixItem->setPixmap(*m_pixMap);
        m_pixItem->setPos(m_pScene->width()/2-m_pixMap->width()/2, m_pScene->height()/2-m_pixMap->height()/2);
        m_curPointer = m_pointer60PR;
        m_curPointer->setVisible(true);

    }
    else if (arg1 == "100PR")
    {
        m_pixMap = m_pixMap100PR;
        m_pixItem->setPixmap(*m_pixMap);
        m_pixItem->setPos(m_pScene->width()/2-m_pixMap->width()/2, m_pScene->height()/2-m_pixMap->height()/2);
        qDebug()<<"100PR";
        m_curPointer = m_pointer100PR;
        m_curPointer->setVisible(true);
    }
    else if (arg1 == "HM100PR")
    {
        m_pixMap = m_pixMapHM100PR;
        m_pixItem->setPixmap(*m_pixMap);
        m_pixItem->setPos(m_pScene->width()/2-m_pixMap->width()/2, m_pScene->height()/2-m_pixMap->height()/2);
        m_curPointer = m_pointerHM100PR;
        m_curPointer->setVisible(true);
    }
    else if (arg1 == "RDH")
    {
        m_pixMap = m_pixMapDT;
        m_pixItem->setPixmap(*m_pixMap);
        m_pixItem->setPos(m_pScene->width()/2-m_pixMap->width()/2, m_pScene->height()/2-m_pixMap->height()/2);
        m_txtItemD->setVisible(true);
        m_txtItemP->setVisible(true);
        m_txtItemT->setVisible(true);
        m_txtItemPpm->setVisible(true);
        m_txtItemOP->setVisible(true);
        m_txtItemUP->setVisible(true);
    }
    else if (arg1 == "100PRW_1")
    {
        m_pixMap = m_pixMap100PRW_1;
        m_pixItem->setPixmap(*m_pixMap);
        m_pixItem->setPos(m_pScene->width()/2-m_pixMap->width()/2, m_pScene->height()/2-m_pixMap->height()/2);
        m_curPointer = m_pointer100PRW_1;
        m_curPointer->setVisible(true);
    }
    else if (arg1 == "100PRW_2")
    {
        m_pixMap = m_pixMap100PRW_2;
        m_pixItem->setPixmap(*m_pixMap);
        m_pixItem->setPos(m_pScene->width()/2-m_pixMap->width()/2, m_pScene->height()/2-m_pixMap->height()/2);
        m_curPointer = m_pointer100PRW_2;
        m_curPointer->setVisible(true);
    }

    int nDMJType = 0;
    bool isExist = false;
    nDMJType = Arg2DMJType(arg1);
    for (int i = 0; i < m_vctDMJData->size(); i++)
    {
        if (m_vctDMJData->at(i).nDMJType == nDMJType)
        {
            isExist = true;
            break;
        }
    }
    if (isExist == false)
    {
        ui->lineEdit_address->setText("-1");
        ui->lineEdit_type->setText("(该表未连接)");
    }
    else
    {
        for (int i = 0; i < m_vctDMJData->size(); i++)
        {
            int t = m_vctDMJData->at(i).T;
            short s16T = (short)t;
            int p = m_vctDMJData->at(i).P;
            int d = m_vctDMJData->at(i).D;
            int address = m_vctDMJData->at(i).nAddress;
            int nPpm = m_vctDMJData->at(i).Ppm;
            int nOP = m_vctDMJData->at(i).nOP;
            short s16OP = (short)nOP;
            int nUP = m_vctDMJData->at(i).nUP;
            short s16UP = (short)nUP;
            if (m_vctDMJData->at(i).nDMJType == nDMJType)
            {
                m_curDMJData = &m_vctDMJData->operator[](i);
                if (nDMJType == DMJ60PR_1)
                {
                    ui->lineEdit_D->setText(QString::asprintf("%.1f", (float)d/10) + "KPa");
                    ui->lineEdit_T->setText(QString::asprintf("%.2f", (float)s16T/100) + "℃");
                    ui->lineEdit_P->setText(QString::asprintf("%.1f", (float)p/10) + "KPa");
                }
                else
                {
                    ui->lineEdit_D->setText(QString::asprintf("%.3f", (float)d/10000) + "MPa");
                    ui->lineEdit_T->setText(QString::asprintf("%.2f", (float)s16T/100) + "℃");
                    ui->lineEdit_P->setText(QString::asprintf("%.3f", (float)p/10000) + "MPa");
                }
                ui->lineEdit_address->setText(QString::number(address));
                /* 控制表盘指针 */
                float angle;
                if (nDMJType == DMJ60PR_1)
                {
                    angle = d/2000.0*270 + 45;
                    m_curPointer->setRotation(angle);
                }
                else if (nDMJType != DMJ_RDH_1)
                {
                    angle = d/10000.0*270 + 45;
                    m_curPointer->setRotation(angle);
                }
                else
                {
                    m_txtItemT->setText("温度:" + QString::asprintf("%.2f", (float)s16T/100) + "℃");
                    m_txtItemD->setText("密度:" + QString::asprintf("%.3f", (float)d/10000) + "MPa");
                    m_txtItemP->setText("压力:" + QString::asprintf("%.3f", (float)p/10000) + "MPa");
                    m_txtItemPpm->setText("微水:" + QString::number(nPpm) + "ppm");
                    m_txtItemOP->setText("常压露点:" + QString::asprintf("%.2f", (float)s16OP/100) + "℃");
                    m_txtItemUP->setText("带压露点" + QString::asprintf("%.2f", (float)s16UP/100) + "℃");
                }

            }
        }
    }

}

void SingleDMJForm::on_comboBox_type_currentIndexChanged(int index)
{
    switch(index)
    {
        case 0:
            qDebug()<<0;
            break;
        case 1:
            qDebug()<<1;
            break;
        case 2:
            qDebug()<<2;
            break;
        case 3:
            qDebug()<<3;
            break;
        case 4:
            qDebug()<<4;
            break;
        case 5:
            qDebug()<<5;
            break;
        case 6:
            qDebug()<<6;
            break;

    }
}

void SingleDMJForm::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        qDebug()<<event->pos();
        m_origin.setX(event->pos().x());
        m_origin.setY(event->pos().y());
        qDebug()<<m_eclipse->boundingRect().center();
        qDebug()<<"p"<<m_curPointer->boundingRect();

    }
}

void SingleDMJForm::slotRcvRtData(QJsonObject *data, DeviceSymbolInfo info)
{

    if (nullptr != data)
    {
        int t = data->value("温度").toInt();
        short s16T = (short)t;
        int p = data->value("压力").toInt();
        int d = data->value("密度").toInt();
        int address = data->value("地址").toInt();
        int nPpm = data->value("Ppm").toInt();
        int nOP = data->value("常压露点").toInt();
        short s16OP = (short)nOP;
        int nUP = data->value("带压露点").toInt();
        short s16UP = (short)nUP;

        bool isExist = false;
        int nFirstFlag = m_vctDMJData->size();
        for (int i = 0; i < m_vctDMJData->size(); i++)
        {
            if (m_vctDMJData->at(i).nAddress == address)
            {
                m_vctDMJData->operator[](i).D = d;
                m_vctDMJData->operator[](i).T = t;
                m_vctDMJData->operator[](i).P = p;
                m_vctDMJData->operator[](i).Ppm = nPpm;
                m_vctDMJData->operator[](i).nOP = nOP;
                m_vctDMJData->operator[](i).nUP = nUP;
                isExist = true;
                break;
            }
        }
        if (!isExist)
        {
            TDMJData td;
            td.D = d;
            td.T = t;
            td.P = p;
            td.nOP = nOP;
            td.nUP = nUP;
            td.nAddress = address;
            td.nType = info.nDeviceType;
            td.nDMJType = info.nDMJType;
            m_vctDMJData->append(td);
        }
        int lastAddr = ui->lineEdit_address->text().toInt();
        qDebug()<<"lastAddr = "<<lastAddr;
        if (address == lastAddr || lastAddr == 0 || !nFirstFlag)
        {
            ui->lineEdit_address->setText(QString::number(address));
            if (info.nDMJType == DMJ60PR_1)
            {
                ui->lineEdit_D->setText(QString::asprintf("%.1f", (float)d/10) + "KPa");
                ui->lineEdit_T->setText(QString::asprintf("%.2f", (float)s16T/100) + "℃");
                ui->lineEdit_P->setText(QString::asprintf("%.1f", (float)p/10) + "KPa");
            }
            else
            {
                ui->lineEdit_D->setText(QString::asprintf("%.3f", (float)d/10000) + "MPa");
                ui->lineEdit_T->setText(QString::asprintf("%.2f", (float)s16T/100) + "℃");
                ui->lineEdit_P->setText(QString::asprintf("%.3f", (float)p/10000) + "MPa");
            }

            qDebug()<<"info.nDMJType"<<info.nDMJType;
            ui->lineEdit_type->setText(QString::number(info.nDMJType));
            /* 控制表盘指针 */
            float angle;
            if (info.nDMJType == DMJ60PR_1)
            {
                angle = d/2000.0*270 + 45;
                m_curPointer->setRotation(angle);
            }
            else if (info.nDMJType != DMJ_RDH_1)
            {
                angle = d/10000.0*270 + 45;
                m_curPointer->setRotation(angle);
            }
            else
            {
                m_txtItemT->setText("温度:" + QString::asprintf("%.2f", (float)s16T/100) + "℃");
                m_txtItemD->setText("密度:" + QString::asprintf("%.3f", (float)d/10000) + "MPa");
                m_txtItemP->setText("压力:" + QString::asprintf("%.3f", (float)p/10000) + "MPa");
                m_txtItemPpm->setText("微水:" + QString::number(nPpm) + "ppm");
                m_txtItemOP->setText("常压露点:" + QString::asprintf("%.2f", (float)s16OP/100) + "℃");
                m_txtItemUP->setText("带压露点:" + QString::asprintf("%.2f", (float)s16UP/100) + "℃");
            }

        }

    }

}

void SingleDMJForm::slotChangeCombText(QString str)
{
    ui->comboBox_type->setCurrentText(str);
}
