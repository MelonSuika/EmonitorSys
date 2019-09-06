#include "rtchartform.h"
#include "ui_rtchartform.h"
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QDateTimeAxis>
#include <QtGui/QResizeEvent>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsTextItem>
#include <QtGui/QMouseEvent>
#include "callout.h"
#include <QGraphicsView>
#include <QGraphicsScene>

RtChartForm::RtChartForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RtChartForm),
    m_coordX(0),
    m_coordY(0),
    m_tooltip(0)
{
    ui->setupUi(this);

    /* 部分控件不显示 */
    ui->dateTimeEdit->hide();
    ui->dateTimeEdit_2->hide();
    ui->label->hide();
    ui->label_2->hide();

    /* 下拉框初始 */
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        ui->comboBox->addItem(info.portName());
    }


    /* 按钮颜色 */
    ui->pushButton_density->font();

    /* 画图 */
    /* 压力曲线 */
    m_Series = new QLineSeries();
    m_Series->setPen(QPen(Qt::blue,1,Qt::SolidLine));
    /* 密度曲线 */
    m_SeriesDensity = new QLineSeries;
    m_SeriesDensity->setPen(QPen(Qt::yellow,1,Qt::SolidLine));

    /* 温度曲线 */
    m_SeriesTemperature = new QSplineSeries;
    m_SeriesTemperature->setPen(QPen(Qt::magenta,1,Qt::SolidLine));

    /* 曲线名 */
    m_Series->setName("压力");
    m_SeriesDensity->setName("密度");
    m_SeriesTemperature->setName("温度");

    /* chart中添加曲线 */
    m_Chart = new QChart;
    m_Chart->addSeries(m_Series);
    m_Chart->addSeries(m_SeriesDensity);
    m_Chart->addSeries(m_SeriesTemperature);

    /* 图例 */
    m_Chart->legend()->setVisible(true);
    m_Chart->legend()->setBackgroundVisible(true);
    m_Chart->legend()->setAlignment(Qt::AlignBottom);
    m_Chart->legend()->show();
    m_Chart->setTitle("曲线图");
    m_Chart->setAnimationOptions(QChart::SeriesAnimations);


    /* expanding */
    m_Chart->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);


    m_ChartView = new QChartView(m_Chart);

    /* X轴 */
    m_dateTimeAxisX = new QDateTimeAxis();
    m_dateTimeAxisX->setTitleText("时间");
    m_dateTimeAxisX->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(60));

    m_dateTimeAxisX->setFormat("H:mm:ss");
    m_dateTimeAxisX->setLabelsAngle(60);
    m_dateTimeAxisX->setTickCount(12);


    m_ChartView->chart()->addAxis(m_dateTimeAxisX, Qt::AlignBottom);
    m_ChartView->chart()->setTheme(QtCharts::QChart::ChartThemeDark);

    /* Y轴(压力)设置 */
    m_axnisY = new QValueAxis;
    m_axnisY->setTitleText("压力(MPa)");
    m_axnisY->setRange(-0.200, 0.900);
    m_axnisY->setLabelFormat("%.3f");
    m_axnisY->setTickCount(11);
    m_axnisY->setLabelsVisible(true);
    m_axnisY->show();
    m_ChartView->chart()->addAxis(m_axnisY, Qt::AlignLeft);
    m_ChartView->chart()->setAxisY(m_axnisY, m_Series);

    /* Y轴(密度)设置 */
    m_axnisYDensity = new QValueAxis;
    m_axnisYDensity->setTitleText("密度(MPa)");
    m_axnisYDensity->setRange(-0.100, 0.900);
    m_axnisYDensity->setLabelFormat("%.3f");
    m_axnisYDensity->setTickCount(11);
    //m_axnisYDensity->setLabelsColor(QColor(0, 0, 255));
    //m_axnisYDensity->setLabelsVisible(true);
    //m_axnisYDensity->hide();
    m_ChartView->chart()->addAxis(m_axnisYDensity, Qt::AlignLeft);
    m_ChartView->chart()->setAxisY(m_axnisYDensity, m_SeriesDensity);

    /* Y轴(温度)设置 */
    m_axnisYTemperature = new QValueAxis;
    m_axnisYTemperature->setTitleText("温度(℃)");
    m_axnisYTemperature->setRange(-40.00, 70.00);
    m_axnisYTemperature->setLabelFormat("%.2f");
    m_axnisYTemperature->setTickCount(11);
    //m_axnisYTemperature->setVisible(false);
    m_ChartView->chart()->addAxis(m_axnisYTemperature, Qt::AlignLeft);
    m_ChartView->chart()->setAxisY(m_axnisYTemperature, m_SeriesTemperature);

    /* 曲线绑定坐标轴 */
    m_Series->attachAxis(m_dateTimeAxisX);
    m_Series->attachAxis(m_axnisY);
    m_SeriesDensity->attachAxis(m_dateTimeAxisX);
    m_SeriesDensity->attachAxis(m_axnisYDensity);
    m_SeriesTemperature->attachAxis(m_dateTimeAxisX);
    m_SeriesTemperature->attachAxis(m_axnisYTemperature);

    /* 副属性不可见，切换时可见 */
    //m_SeriesDensity->hide();
    //m_SeriesTemperature->hide();


    m_ChartView->setRenderHint(QPainter::Antialiasing);
    m_ChartView->resize(800, 600);
    m_ChartView->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    ui->verticalLayout_chart->addWidget(m_ChartView);


    m_nXIndex = 0;

#if 0
    /*new */
    m_coordX = new QGraphicsSimpleTextItem(m_Chart);
    m_coordX->setPos(m_Chart->size().width()/2 - 50, m_Chart->size().height());
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(m_Chart);
    m_coordY->setPos(m_Chart->size().width()/2 + 50, m_Chart->size().height());
    m_coordY->setText("Y: ");
#endif
    //connect(m_Series, &QLineSeries::clicked, this, &RtChartForm::keepCallout);
    connect(m_Series, &QLineSeries::hovered, this, &RtChartForm::tooltip);

    //connect(m_SeriesDensity, &QSplineSeries::clicked, this, &RtChartForm::keepCallout);
    connect(m_SeriesDensity, &QSplineSeries::hovered, this, &RtChartForm::tooltip);

    //connect(m_SeriesTemperature, &QSplineSeries::clicked, this, &RtChartForm::keepCallout);
    connect(m_SeriesTemperature, &QSplineSeries::hovered, this, &RtChartForm::tooltip);

    this->setMouseTracking(false);

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
        m_dateTimeAxisX->setMin(QDateTime::fromMSecsSinceEpoch(x - m_dateTimeAxisX->max().toMSecsSinceEpoch() + m_dateTimeAxisX->min().toMSecsSinceEpoch()));
        m_dateTimeAxisX->setMax(QDateTime::currentDateTime());

    }
    m_Series->append(x, (float)p/10000 - 0.1);
    m_SeriesDensity->append(x, (float)c/10000 - 0.1);
    m_SeriesTemperature->append(x, (float)t/100);

}


void RtChartForm::mouseMoveEvent(QMouseEvent *event)
{
    //m_coordX->setText(QString("X: %1").arg(QDateTime::fromMSecsSinceEpoch(m_Chart->mapToValue(event->pos()).x()).toString()));
    //m_coordY->setText(QString("Y: %1").arg(m_Chart->mapToValue(event->pos()).y()));
    //QGraphicsView::mouseMoveEvent(event);
}

void RtChartForm::keepCallout()
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_Chart);
}

void RtChartForm::tooltip(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(m_Chart);

    if (state) {
        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(QDateTime::fromMSecsSinceEpoch(point.x()).toString()).arg(point.y()));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}



void RtChartForm::on_pushButton_temperature_clicked()
{
    if (m_SeriesTemperature->isVisible())
    {
        //m_axnisY->setLabelFormat("%.3f");
        //m_axnisY->setRange(-0.100, 0.900);

        m_SeriesTemperature->hide();
        //m_SeriesDensity->hide();
        //m_Series->show();
    }
    else
    {
        //m_axnisY->setTitleText("温度(℃)");
        //m_axnisY->setLabelFormat("%.2f");
        //m_axnisY->setRange(-40.00, 70.00);

        //m_Series->hide();
        //m_SeriesDensity->hide();
        m_SeriesTemperature->show();

    }

}

void RtChartForm::on_pushButton_density_clicked()
{
    if (m_SeriesDensity->isVisible())
    {
        //m_axnisY->setRange(-0.100, 0.900);
        m_SeriesDensity->hide();
        //m_SeriesTemperature->hide();
        //m_Series->show();
    }
    else
    {
        //m_axnisY->setTitleText("密度(MPa)");
        //m_axnisY->setRange(-0.100, 0.900);

       // m_Series->hide();
        //m_SeriesTemperature->hide();
        m_SeriesDensity->show();
    }

}

void RtChartForm::on_pushButton_allDisplay_clicked()
{
    m_Series->show();
    m_SeriesDensity->show();
    m_SeriesTemperature->show();

}

void RtChartForm::on_pushButton_pressure_clicked()
{

    if (m_Series->isVisible())
    {
        m_Series->hide();
    }
    else
    {
        //m_SeriesTemperature->hide();
        //m_SeriesDensity->hide();
        m_Series->show();
    }
}
