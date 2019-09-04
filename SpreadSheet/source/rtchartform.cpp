#include "rtchartform.h"
#include "ui_rtchartform.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDateTimeAxis>

RtChartForm::RtChartForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RtChartForm)
{
    ui->setupUi(this);

    /* 下拉框初始 */
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(info.portName());
    }

    /* 画图 */
    m_Series = new QLineSeries;
    m_Series->setPen(QPen(Qt::blue,1,Qt::SolidLine)); //设置画笔

    m_Chart = new QChart;
    m_Chart->addSeries(m_Series);
    //m_Chart->legend()->hide();
    m_Chart->setTitle("曲线图");
    //m_Chart->createDefaultAxes();

    /* expanding */
    m_Chart->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);


    m_ChartView = new QChartView(m_Chart);
#if 0
    m_axnisX = new QValueAxis;
    m_axnisX->setTitleText("时间");
    m_axnisX->setRange(0, 24);
    m_axnisX->setTickCount(7);
    m_axnisX->setLabelFormat("%u");
    m_axnisX->setGridLineVisible(true);
    m_axnisX->setMinorTickCount(1);
#else
    m_dateTimeAxisX = new QDateTimeAxis();
    m_dateTimeAxisX->setTitleText("时间");
    m_dateTimeAxisX->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(60));

    m_dateTimeAxisX->setFormat("H:mm:ss");
    m_dateTimeAxisX->setLabelsAngle(60);
    m_dateTimeAxisX->setTickCount(15);
#endif

    m_ChartView->chart()->addAxis(m_dateTimeAxisX, Qt::AlignBottom);
    m_Series->attachAxis(m_dateTimeAxisX);

    m_ChartView->chart()->legend()->hide();
    m_ChartView->chart()->setTheme(QtCharts::QChart::ChartThemeDark);

    /* Y轴设置 */
    m_axnisY = new QValueAxis;
    m_axnisY->setTitleText("压力(MPa)");
    m_axnisY->setRange(-0.100, 0.900);
    m_axnisY->setLabelFormat("%.2f");
    m_axnisY->setMinorTickCount(0.01);
    m_ChartView->chart()->setAxisY(m_axnisY, m_Series);

    m_ChartView->setRenderHint(QPainter::Antialiasing);
    m_ChartView->resize(800, 600);
    m_ChartView->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    ui->verticalLayout_chart->addWidget(m_ChartView);


    m_nXIndex = 0;

}

RtChartForm::~RtChartForm()
{
    delete ui;
}

void RtChartForm::rcvRtData(QJsonObject *data, int nDeviceType)
{
    //qDebug()<<"eee"<<data->value("压力").toInt();
    int p = data->value("压力").toInt();
    int t = data->value("温度").toInt();
    int c = data->value("密度").toInt();

    qint64 x = QDateTime::currentMSecsSinceEpoch();
    /* 当前时间大于最大x坐标，当前时间设为x最大坐标 */
    if (x > m_dateTimeAxisX->max().toMSecsSinceEpoch())
    {
        //qDebug()<<"tttt"<<QDateTime::fromMSecsSinceEpoch(x - m_dateTimeAxisX->max().toMSecsSinceEpoch() + m_dateTimeAxisX->min().toMSecsSinceEpoch());
        m_dateTimeAxisX->setMin(QDateTime::fromMSecsSinceEpoch(x - m_dateTimeAxisX->max().toMSecsSinceEpoch() + m_dateTimeAxisX->min().toMSecsSinceEpoch()));
        m_dateTimeAxisX->setMax(QDateTime::currentDateTime());

    }
    m_Series->append(x, (float)p/10000);

}


