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
#include <QStringListModel>


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
    ui->pushButton_clear->hide();
    ui->comboBox->hide();
    ui->lineEdit_addr->hide();

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
    m_dateTimeAxisX->setGridLineVisible(false);
    // m_dateTimeAxisX->setGridLinePen(QPen(QBrush(QColor(255, 255, 255, 55)), 1, Qt::DotLine));

    m_dateTimeAxisX->setFormat("H:mm:ss");
    m_dateTimeAxisX->setLabelsAngle(60);
    m_dateTimeAxisX->setTickCount(12);

    m_ChartView->chart()->addAxis(m_dateTimeAxisX, Qt::AlignBottom);
    m_ChartView->chart()->setTheme(QtCharts::QChart::ChartThemeDark);


    /* Y轴(压力)设置 */
    m_axnisY = new QValueAxis;
    m_axnisY->setTitleText("压力(MPa)");
    m_axnisY->setRange(-0.100, 0.900);
    m_axnisY->setLabelFormat("%.3f");
    m_axnisY->setTickCount(11);
    m_axnisY->setLabelsVisible(true);
    m_axnisY->setGridLineVisible(false);
    //m_axnisY->show();
    m_axnisY->setVisible(false); // 将压力轴隐去，合并到Y轴使图看起来更舒服
    m_ChartView->chart()->addAxis(m_axnisY, Qt::AlignLeft);
    m_ChartView->chart()->setAxisY(m_axnisY, m_Series);


    /* Y轴(密度)设置 */
    m_axnisYDensity = new QValueAxis;
    m_axnisYDensity->setTitleText("密度/压力(MPa)");
    m_axnisYDensity->setRange(-0.100, 0.900);
    m_axnisYDensity->setLabelFormat("%.3f");
    m_axnisYDensity->setTickCount(11);
    m_axnisYDensity->setGridLineVisible(false);
    m_ChartView->chart()->addAxis(m_axnisYDensity, Qt::AlignLeft);
    m_ChartView->chart()->setAxisY(m_axnisYDensity, m_SeriesDensity);

    /* Y轴(温度)设置 */
    m_axnisYTemperature = new QValueAxis;
    m_axnisYTemperature->setTitleText("温度(℃)");
    m_axnisYTemperature->setRange(-40.00, 70.00);
    m_axnisYTemperature->setLabelFormat("%.2f");
    m_axnisYTemperature->setTickCount(11);
    m_axnisYTemperature->setGridLinePen(QPen(QBrush(QColor(255, 255, 255, 55)), 1, Qt::DotLine));
    m_ChartView->chart()->addAxis(m_axnisYTemperature, Qt::AlignLeft);
    m_ChartView->chart()->setAxisY(m_axnisYTemperature, m_SeriesTemperature);

    /* 曲线绑定坐标轴 */
    m_Series->attachAxis(m_dateTimeAxisX);
    m_SeriesDensity->attachAxis(m_dateTimeAxisX);
    m_SeriesTemperature->attachAxis(m_dateTimeAxisX);

    /* 副属性不可见，切换时可见 */
    //m_SeriesDensity->hide();
    //m_SeriesTemperature->hide();


    m_ChartView->setRenderHint(QPainter::Antialiasing);
    m_ChartView->resize(800, 600);
    m_ChartView->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
    ui->verticalLayout_chart->addWidget(m_ChartView);

    /* 曲线图外边框(白色区域)缩小,与layout相邻部分 */
    m_ChartView->chart()->setContentsMargins(-8, -8, -8, -8);

    /* 左右大窗横向比例 */
    ui->gridLayout->setColumnStretch(0, 5);
    ui->gridLayout->setColumnStretch(1, 1);

    /* 报警框初始化 */
    m_strlistAlert = new QStringList;
    m_strListModelAlert = new QStringListModel;
    m_strListModelAlert->setStringList(*m_strlistAlert);
    ui->listView_alert->setModel(m_strListModelAlert);

    m_nXIndex = 0;
    m_nCurAddr = -1;

    m_vctRTData = new QVector<TRtChartData>;

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

void RtChartForm::rcvRtData(QJsonObject *data, DeviceSymbolInfo deviceSbInfo)
{
    qDebug()<<"RtChartForm::rcvRtData"<<deviceSbInfo.nDMJType;
#if 0
    /* 这边大于0返回是为了只显示第一个表的曲线 */
    if (deviceSbInfo.nDeviceIndex > 0)
        return;
#endif
    bool isExist = false;
    int p = data->value("压力").toInt();
    int t = data->value("温度").toInt();
    int c = data->value("密度").toInt();
    int addr = data->value("地址").toInt();
    int ppm = data->value("Ppm").toInt();
    qint64 x = QDateTime::currentMSecsSinceEpoch();
    int nSize = m_vctRTData->size();
    for (int i = 0; i < m_vctRTData->size(); i++)
    {
        TRtChartData *ptRtChartData = &m_vctRTData->operator[](i);
        if (ptRtChartData->nAddr == addr)
        {
            TDMJData tData;
            tData.D = c;
            tData.P = p;
            tData.T = t;
            tData.Ppm = ppm;
            ptRtChartData->pMapData->insert(x, tData);
            if (ptRtChartData->pMapData->size() > 605)
            {
                ptRtChartData->pMapData->erase(ptRtChartData->pMapData->begin());
            }
            isExist = true;
        }
    }


    /* if not exist, add it */
    if (false == isExist)
    {
        TRtChartData *ptRtChartData = new TRtChartData;
        ptRtChartData->pMapData = new QMap<qint64, TDMJData>;
        TDMJData tData;
        tData.D = c;
        tData.P = p;
        tData.T = t;
        tData.Ppm = ppm;
        ptRtChartData->pMapData->insert(x, tData);
        ptRtChartData->nAddr = addr;
        ptRtChartData->nType = deviceSbInfo.nDeviceType;
        ptRtChartData->nDMJType = deviceSbInfo.nDMJType;
        ui->comboBox_addrList->addItem(QString::number(addr));
        m_vctRTData->append(*ptRtChartData);
    }


    /* 当前时间大于最大x坐标，当前时间设为x最大坐标 */
    if (x > m_dateTimeAxisX->max().toMSecsSinceEpoch())
    {
        m_dateTimeAxisX->setMin(QDateTime::fromMSecsSinceEpoch(x - m_dateTimeAxisX->max().toMSecsSinceEpoch() + m_dateTimeAxisX->min().toMSecsSinceEpoch()));
        //m_dateTimeAxisX->setMax(QDateTime::currentDateTime());
        m_dateTimeAxisX->setMax(QDateTime::fromMSecsSinceEpoch(x));

        if (m_Series->points().size() > 600)
        {
            m_Series->remove(m_Series->points().first());
            m_SeriesDensity->remove(m_SeriesDensity->points().first());
            m_SeriesTemperature->remove(m_SeriesTemperature->points().first());
        }

    }
    /* 警报信息记录 */
    if (p > 6000 || t > 6000 || c > 6000 && m_strListModelAlert->rowCount() < 200)
    {
        QString str = QDateTime::currentDateTime().toString() + "压力:" + QString::number(p) + "密度:" + QString::number(c) + "温度:" + QString::number(t);
        m_strListModelAlert->insertRow(m_strListModelAlert->rowCount());
        QModelIndex index = m_strListModelAlert->index(m_strListModelAlert->rowCount()-1, 0);
        m_strListModelAlert->setData(index, str, Qt::DisplayRole);
    }

    /* update ui */
    if (0 == nSize || addr == m_nCurAddr)
    {
        m_Series->append(x, p/10000.0);
        m_SeriesDensity->append(x, c/10000.0);
        m_SeriesTemperature->append(x, t/100.0);
        m_nCurAddr = addr;

    }

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
        m_SeriesTemperature->hide();
    }
    else
    {
        m_SeriesTemperature->show();

    }

}

void RtChartForm::on_pushButton_density_clicked()
{
    if (m_SeriesDensity->isVisible())
    {
        m_SeriesDensity->hide();
    }
    else
    {
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
        m_Series->show();
    }
}

void RtChartForm::on_pushButton_clear_clicked()
{
    m_Series->clear();
}

/*
 * function:
 * Change current address,and then
 * switch to matched curve
*/
void RtChartForm::on_comboBox_addrList_currentTextChanged(const QString &arg1)
{
    int nChangeAddr = arg1.toInt();
    qint64 x = QDateTime::currentMSecsSinceEpoch();
    qint64 minX = x - m_dateTimeAxisX->max().toMSecsSinceEpoch() + m_dateTimeAxisX->min().toMSecsSinceEpoch();
    /* 当前时间大于最大x坐标，当前时间设为x最大坐标 */
    if (x > m_dateTimeAxisX->max().toMSecsSinceEpoch())
    {
        m_dateTimeAxisX->setMin(QDateTime::fromMSecsSinceEpoch(minX));
        m_dateTimeAxisX->setMax(QDateTime::fromMSecsSinceEpoch(x));

        if (m_Series->points().size() > 600)
        {
            m_Series->remove(m_Series->points().first());
            m_SeriesDensity->remove(m_SeriesDensity->points().first());
            m_SeriesTemperature->remove(m_SeriesTemperature->points().first());
        }

    }
    if (nChangeAddr == m_nCurAddr)
    {
        return;
    }
    else
    {
        m_nCurAddr = nChangeAddr;
        m_Series->clear();
        m_SeriesDensity->clear();
        m_SeriesTemperature->clear();
        int nCount = 0;
        for (int i = 0; i < m_vctRTData->size(); i++)
        {
            if (m_vctRTData->at(i).nAddr == m_nCurAddr)
            {
                QMap<qint64, TDMJData>::iterator it = m_vctRTData->operator[](i).pMapData->begin();
                while(it != m_vctRTData->operator[](i).pMapData->end())
                {
                    if (it.key() >= minX)
                    {
                        m_Series->append(it.key(), it.value().P/10000.0);
                        m_SeriesDensity->append(it.key(), it.value().D/10000.0);
                        m_SeriesTemperature->append(it.key(), it.value().T/100.0);
                        nCount++;
                        if (nCount >= 600)
                        {
                            break;
                        }
                    }
                    it++;
                }

            }
        }
    }
}

/*
    function:
    make address into qstring arguments
*/
void RtChartForm::slotAddr2Arg(QString strType)
{
    qDebug()<<"----------------"<<strType;
    for (int i = 0; i < m_vctRTData->size(); i++)
    {
        if (m_vctRTData->at(i).nDMJType == Arg2DMJType(strType))
        {
            ui->comboBox_addrList->setCurrentText(QString::number(m_vctRTData->at(i).nAddr));
            break;
        }
    }
}
