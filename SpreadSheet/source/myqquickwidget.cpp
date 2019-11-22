#include "myqquickwidget.h"
#include <QTimer>
#include <QQmlEngine>
#include <QQmlContext>

MyQQuickWidget::MyQQuickWidget()
{
    m_gasTimer = new QTimer(this);

    connect(m_gasTimer, SIGNAL(timeout()), this, SLOT(rcvData2Qml()));

}

bool MyQQuickWidget::event(QEvent *e)
{
    if (e->type() == QEvent::Resize)
    {
        emit setQuickWidgetContext();
    }
    return QWidget::event(e);

}

void MyQQuickWidget::rcvData2Qml()
{

    this->engine()->rootContext()->setContextProperty("gasValue", (rand()%10000)/10.0);

}

void MyQQuickWidget::startRcv()
{

    m_gasTimer->start(1000);

}
