#include "rtchartform.h"
#include "ui_rtchartform.h"
#include <QSerialPortInfo>
#include <QSerialPort>

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

#if 0
    *m_Series << QPointF(1, 5) << QPointF(3, 7) << QPointF(7, 6) << QPointF(9, 7) << QPointF(12, 6)
            << QPointF(16, 7) << QPointF(18, 5);
#endif

    m_Chart = new QChart;
    m_Chart->addSeries(m_Series);
    //m_Chart->legend()->hide();
    m_Chart->setTitle("曲线图");
    m_Chart->createDefaultAxes();

    m_ChartView = new QChartView(m_Chart);
    m_axnisX = new QValueAxis;
    m_axnisX->setTitleText("时间");
    m_axnisX->setRange(0, 24);
    m_axnisX->setTickCount(7);
    m_axnisX->setLabelFormat("%u");
    m_axnisX->setGridLineVisible(true);
    m_axnisX->setMinorTickCount(1);


    m_ChartView->chart()->setAxisX(m_axnisX, m_Series);

    m_axnisY = new QValueAxis;
    m_axnisY->setTitleText("压力");
    m_axnisY->setRange(0, 10);
    m_axnisY->setLabelFormat("%u");
    m_axnisX->setGridLineVisible(true);
    m_axnisY->setMinorTickCount(1);
    m_ChartView->chart()->setAxisY(m_axnisY, m_Series);

    m_ChartView->setRenderHint(QPainter::Antialiasing);
    m_ChartView->resize(800, 600);
    m_ChartView->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    ui->verticalLayout_chart->addWidget(m_ChartView);

}

RtChartForm::~RtChartForm()
{
    delete ui;
}
