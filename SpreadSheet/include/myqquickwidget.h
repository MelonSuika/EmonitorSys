#ifndef MYQQUICKWIDGET_H
#define MYQQUICKWIDGET_H

#include <QObject>
#include <QQuickWidget>

class MyQQuickWidget : public QQuickWidget
{
    Q_OBJECT
public:
    MyQQuickWidget();

    bool event(QEvent *e);
    void startRcv();

signals:
    void setQuickWidgetContext();
    void sendData2Qml(int);

private slots:
    void rcvData2Qml();

private:
    QTimer *m_gasTimer;


public:
};

#endif // MYQQUICKWIDGET_H
