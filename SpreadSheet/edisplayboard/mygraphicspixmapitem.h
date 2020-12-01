#ifndef MYGRAPHICSPIXMAPITEM_H
#define MYGRAPHICSPIXMAPITEM_H

#include <QObject>
#include <QGraphicsPixmapItem>

class MyGraphicsPixmapItem :  public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    MyGraphicsPixmapItem();
    MyGraphicsPixmapItem(QString);
    void focusInEvent(QFocusEvent *e);
    void focusOutEvent(QFocusEvent *e);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
signals:
    void sigSendDMJType(QString);
private:
    int m_nDMJType;
    QString m_strDMJType;

};

#endif // MYGRAPHICSPIXMAPITEM_H
