 #include "chartform.h"
#include "ui_chartform.h"
#include <QValueAxis>


QT_CHARTS_USE_NAMESPACE
ChartForm::ChartForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChartForm)
{
    ui->setupUi(this);

    /* point list */
    m_vectorPoint = new QVector<QPointF>;

    /* 加载qss改变界面风格 */
    QFile qssfile(":/qss/widget-blue.qss");
    qssfile.open(QFile::ReadOnly);
    QString qss;
    qss = qssfile.readAll();
    this->setStyleSheet(qss);


    m_series = new QSplineSeries();
    m_series->setPen(QPen(Qt::blue,1,Qt::SolidLine)); //设置画笔

    *m_series << QPointF(1, 5) << QPointF(3, 7) << QPointF(7, 6) << QPointF(9, 7) << QPointF(12, 6)
            << QPointF(16, 7) << QPointF(18, 5);

    m_chart = new QChart();
    m_chart->addSeries(m_series);
    m_chart->createDefaultAxes();   //设置默认的坐标系，如果不设置程序是运行不了的
    m_chart->setTitle("曲线图");

    /* 画坐标轴X */
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(0, 23);    //设置范围
    axisX->setLabelFormat("%u");   //设置刻度的格式
    axisX->setGridLineVisible(true);   //网格线可见
    axisX->setTickCount(7);       //设置多少格
    axisX->setMinorTickCount(3);   //设置每格小刻度线的数目
    m_chart->setAxisX(axisX, m_series);

    /* 画坐标轴Y */
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 50);
    axisY->setLabelFormat("%u");
    axisY->setGridLineVisible(true);
    axisY->setTickCount(10);
    axisY->setMinorTickCount(1);

    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    m_chartView->resize(800, 600);
    m_chartView->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    ui->horizontalLayoutChart->addWidget(m_chartView);

    /* select info */
    ui->gridLayout_chart->setColumnStretch(0, 5);
    ui->gridLayout_chart->setColumnStretch(1, 2);

    m_nEnd = 19;
    m_chart->axisY()->setMin(0);

}

ChartForm::~ChartForm()
{
    delete ui;
}

void ChartForm::on_pushButton_display_clicked()
{

    m_query = new QSqlQuery;

    qDebug()<<"SELECT time, temperature, humidity FROM TH3\
              WHERE time > '" + ui->dateTimeEdit_start->text() + "' and time < '" + ui->dateTimeEdit_end->text() + "';";
    m_query->exec("SELECT time, temperature, humidity FROM TH3\
                  WHERE time > '" + ui->dateTimeEdit_start->text() + "' and time < '" + ui->dateTimeEdit_end->text() + "';");
    while(m_query->next())
    {
        QString str = m_query->value(1).toString();
        qDebug()<<str;
        if (m_series->count()>100)
        {
            m_series->removePoints(0, 1);
        }
        m_series->append(m_nEnd, m_query->value(1).toInt()/100);
        m_chart->axisX()->setMin(0);
        m_chart->axisX()->setMax(100);
    }


}


void ChartForm::on_dateTimeEdit_start_dateTimeChanged(const QDateTime &dateTime)
{
    m_startDt = dateTime;
    //qDebug() << m_startDt;
}

void ChartForm::on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime)
{
    m_endDt = dateTime;
    //qDebug() << m_endDt;
}

void ChartForm::on_pushButton_rtDisplay_clicked()
{
    m_series->removePoints(0, 1);
    m_series->append(m_nEnd++, qrand()%8);
    m_chart->axisX()->setMin(m_nEnd-18);
    m_chart->axisX()->setMax(m_nEnd);
}
