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
    m_dataDensity_Time = new QVector<QCPGraphData>;
    m_dataPressure_Time = new QVector<QCPGraphData>;
    m_dataTemperature_Time = new QVector<QCPGraphData>;


    // configure bottom axis to show date instead of number:
    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);
    dateTicker->setDateTimeFormat("MM-dd hh:mm:ss");
    ui->customPlot->xAxis->setTicker(dateTicker);
    ui->customPlot->xAxis->setTickLabelRotation(65);

    ui->customPlot->yAxis->setLabel("密度(MPa)");
    //ui->customPlot->yAxis->settick
    ui->customPlot->yAxis2->setVisible(true);
    ui->customPlot->yAxis2->setLabel("温度(℃)");

    /* 添加时间-密度曲线 */
    ui->customPlot->addGraph();
    ui->customPlot->graph()->data()->set(*m_dataDensity_Time);
    ui->customPlot->graph()->setKeyAxis(ui->customPlot->xAxis);
    ui->customPlot->graph()->setValueAxis(ui->customPlot->yAxis);
    ui->customPlot->graph()->setPen(QColor(0, 255, 0));
    ui->customPlot->graph()->setName("密度");
    ui->customPlot->graph()->setBrush(QBrush(QColor(0, 255, 0, 45)));
    ui->customPlot->graph()->setScatterStyle(QCPScatterStyle::ScatterShape::ssSquare);
    ui->customPlot->graph()->setLineStyle(QCPGraph::LineStyle::lsNone);

    /* 添加时间-温度曲线 */
    ui->customPlot->addGraph();
    ui->customPlot->graph()->data()->set(*m_dataTemperature_Time);
    ui->customPlot->graph()->setKeyAxis(ui->customPlot->xAxis);
    ui->customPlot->graph()->setValueAxis(ui->customPlot->yAxis2);
    ui->customPlot->graph()->setPen(QColor(255, 0, 0));
    ui->customPlot->graph()->setName("温度");
    ui->customPlot->graph()->setBrush(QBrush(QColor(255, 0, 0, 45)));
    ui->customPlot->graph()->setScatterStyle(QCPScatterStyle::ScatterShape::ssDisc);
    ui->customPlot->graph()->setLineStyle(QCPGraph::LineStyle::lsNone);

    /* 添加时间-压力曲线 */
    ui->customPlot->addGraph();
    ui->customPlot->graph()->data()->set(*m_dataPressure_Time);
    ui->customPlot->graph()->setKeyAxis(ui->customPlot->xAxis);
    ui->customPlot->graph()->setValueAxis(ui->customPlot->yAxis);
    ui->customPlot->graph()->setPen(QColor(0, 0, 255));
    ui->customPlot->graph()->setName("压力");
    ui->customPlot->graph()->setBrush(QBrush(QColor(0, 0, 255, 45)));
    ui->customPlot->graph()->setScatterStyle(QCPScatterStyle::ScatterShape::ssPlusCircle);
    ui->customPlot->graph()->setLineStyle(QCPGraph::LineStyle::lsNone);

    /* 设置坐标范围 */
    ui->customPlot->xAxis->setRange(QDateTime::currentDateTime().toTime_t(), QDateTime::currentDateTime().toTime_t() + 100);
    ui->customPlot->yAxis->setRange(-0.2, 0.9);
    ui->customPlot->yAxis2->setRange(-40, 70);


    /* 添加小标 */
    //QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    //textTicker->addTick(0.1, "a bit\nlow");
    //textTicker->addTick(0.5, "quite\nhigh");
    //ui->customPlot->yAxis->setTicker(textTicker);

    /* 显示图例 */
    ui->customPlot->legend->setVisible(true);
    // make bottom axes always transfer their ranges top axes:
    //connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    //ui->customPlot->graph(0)->rescaleAxes(false);
    //ui->customPlot->graph(1)->rescaleAxes(false);

    /* 可拉伸、缩放、选中 */
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);

    /* 仅水平方向可缩放改变 */
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal);
    /* 仅水平方向可拖拽 */
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal);

    /* active calendar */
    ui->dateTimeEdit_start->setCalendarPopup(true);
    ui->dateTimeEdit_end->setCalendarPopup(true);

    /* set current datetime */
    ui->dateTimeEdit_start->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_end->setDateTime(QDateTime::currentDateTime());

    m_sqlProcess = new QThread;
    m_sqlProcess->start();
    m_sqlChartFormObj = new SQLChartFormObject;
    m_sqlChartFormObj->moveToThread(m_sqlProcess);


    connect(this, SIGNAL(onLoad(QDateTime, QDateTime)), m_sqlChartFormObj, SLOT(readData(QDateTime, QDateTime)));
    connect(m_sqlChartFormObj, SIGNAL(loadOver(QVector<double>*,QVector<double>*,QVector<double>*,QVector<double>*)),
            this, SLOT(disPlay(QVector<double>*, QVector<double>*,QVector<double>*,QVector<double>*)));
    connect(ui->customPlot, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(custPlotMoveEvent(QMouseEvent*)));

}

ChartForm::~ChartForm()
{
    delete ui;
}

void ChartForm::on_pushButton_display_clicked()
{

    emit onLoad(m_startDt, m_endDt);
    //ui->customPlot->xAxis->setRange(m_startDt, m_endDt);
    ui->customPlot->xAxis->setRange(QDateTime::fromString(ui->dateTimeEdit_start->text(), "yyyy-MM-dd hh:mm").toSecsSinceEpoch(),
                                        QDateTime::fromString(ui->dateTimeEdit_end->text(), "yyyy-MM-dd hh:mm").toSecsSinceEpoch());
    qDebug()<<"emit onLoad";

}


void ChartForm::on_dateTimeEdit_start_dateTimeChanged(const QDateTime &dateTime)
{
    m_startDt = dateTime;
    //qDebug()<<m_startDt;
}

void ChartForm::on_dateTimeEdit_end_dateTimeChanged(const QDateTime &dateTime)
{
    m_endDt = dateTime;
    //qDebug()<<m_endDt;
}

void ChartForm::on_pushButton_rtDisplay_clicked()
{


}

void ChartForm::on_pushButton_testAddData_clicked()
{
    double low = ui->customPlot->xAxis->range().lower;
    double up = ui->customPlot->xAxis->range().upper;
    qDebug()<<low << up << up - low;
    for (int i = low; i <= up; i++)
    {
        QCPGraphData gphData;
        gphData.key = i;
        gphData.value = 0.9/(i%25);
        ui->customPlot->graph(0)->addData(gphData.key, gphData.value);
        ui->customPlot->replot();
    }
}

void ChartForm::disPlay(QVector<double>*d,QVector<double>*p,QVector<double>*t,QVector<double>*dd)
{
    ui->customPlot->graph(0)->setData(*dd, *d);
    ui->customPlot->graph(1)->setData(*dd, *t);
    ui->customPlot->graph(2)->setData(*dd, *p);
    ui->customPlot->replot();

}


void ChartForm::custPlotMoveEvent(QMouseEvent *e)
{
    /* 获取光标位置 */
    int x_pos = e->pos().x();
    int y_pos = e->pos().y();

    /* 转化为坐标系位置 */
    double x_coord = ui->customPlot->xAxis->pixelToCoord(x_pos);
    double y_coord = ui->customPlot->yAxis->pixelToCoord(y_pos);

    QString str = QString("时间:%1").arg(QDateTime::fromTime_t(x_coord).toString());

    for (int i = 0; i < ui->customPlot->xAxis->graphs().count(); i++)
    {
        double y = 0;
        int start = 0, end = ui->customPlot->graph(i)->data()->size(), cur = start;
        /* 二分查找 */
        do{
            if (x_coord <= ui->customPlot->graph(i)->data()->at((start + end)/2)->key)
            {
                end = (start + end) / 2;
            }
            else
            {
                start = (start + end) / 2;
            }
            y = ui->customPlot->graph(i)->data()->at(end)->value;

        }while(end - start > 1);

        /* 显示文本 */
        switch (i)
        {
            case 0:
                str += QString("\n密度:");
                break;
            case 1:
                str += QString("\n温度:");
                break;
            case 2:
                str += QString("\n压力:");
                break;
            default:
                break;

        }
        str += QString::asprintf("%.3f", y);

    }
    QToolTip::showText(cursor().pos(), str, ui->customPlot);


}
