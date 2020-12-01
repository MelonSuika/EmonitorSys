#include "mygraphicspixmapitem.h"
#include <QDebug>

MyGraphicsPixmapItem::MyGraphicsPixmapItem()
{

    m_nDMJType = 0;
}
MyGraphicsPixmapItem::MyGraphicsPixmapItem(QString str)
{
    m_nDMJType = 0;
    m_strDMJType = str;
}


void MyGraphicsPixmapItem::focusInEvent(QFocusEvent *e)
{
    qDebug()<<"in";
}

void MyGraphicsPixmapItem::focusOutEvent(QFocusEvent *e)
{
    qDebug()<<"out";
}


void MyGraphicsPixmapItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"mouse press";
    emit sigSendDMJType(this->m_strDMJType);

}
