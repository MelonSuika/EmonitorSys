#include "historyanalysisform.h"
#include "ui_historyanalysisform.h"
#include <QMetaType>

HistoryAnalysisForm::HistoryAnalysisForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistoryAnalysisForm)
{
    ui->setupUi(this);
    qRegisterMetaType<TransInfo>("TransInfo");
    qRegisterMetaType<TSheetData>("TSheetData");

    m_sqlHistoryAnalysisObj = new SqlHistoryAnalysisObject;
    m_sqlThread = new QThread;
    m_sqlHistoryAnalysisObj->moveToThread(m_sqlThread);
    m_sqlThread->start();

    /***********
     *  表初始化 *
     * **********/
    ui->tableWidget_deviceStatisInfo->setRowCount(65);
    ui->tableWidget_deviceStatisInfo->setColumnCount(5);
    /* 设置列名 */
    QStringList headers;
    headers<< "地址"<<"类型"<<"密度AVG/MAX/MIN"<<"压力(MPa)"<<"温度(℃)";
    ui->tableWidget_deviceStatisInfo->setHorizontalHeaderLabels(headers);


    /***********
     *  图初始化***
     * *********/
    ui->widget_customPlot->xAxis->setLabel("密度(MPa)");
    ui->widget_customPlot->yAxis->setLabel("数量");
    ui->widget_customPlot->xAxis->setRange(-0.2, 0.9);
    // set dark background gradient: 设置暗背景渐变
    QLinearGradient gradient(0, 0, 0, 400);
    gradient.setColorAt(0, QColor(90, 90, 90));// 开始颜色为黑色
    gradient.setColorAt(0.38, QColor(120, 105, 105));
    gradient.setColorAt(1, QColor(70, 70, 70));// 黑色
    ui->widget_customPlot->setBackground(QBrush(gradient));//设置图表背景（用画刷设置）

    // create empty bar chart objects: 这个就是创建柱状图了
    //新版本应该是取消了之前的AddPlottable（不是很清楚，大概这样子）
    //然后直接在new QCPBars的时候指定x，y轴就可以了
    QCPBars *barCount = new QCPBars(ui->widget_customPlot->xAxis, ui->widget_customPlot->yAxis);
    barCount->setName("分布");
    barCount->setPen(QPen(QColor(0, 168, 140).lighter(130)));
    barCount->setBrush(QColor(0, 168, 140));

    // prepare x axis with country labels: //设置x轴标签
    QVector<double> ticks;
    QVector<QString> labels;
    ticks << 0.1 << 0.2 << 0.3 << 0.4 << 0.5 << 0.6 << 0.7;
    labels << "USA" << "Japan" << "Germany" << "France" << "UK" << "Italy" << "Canada";


    //ui->widget_customPlot->
    // Add data:添加数据
    QVector<double> regenData;
    regenData   << 0.06*10.5 << 0.05*5.5 << 0.04*5.5 << 0.06*5.8 << 0.02*5.2 << 0.07*4.2 << 0.25*11.2;

    barCount->setData(ticks, regenData);




    connect(this, SIGNAL(onLoad(TransInfo)), m_sqlHistoryAnalysisObj, SLOT(readData(TransInfo)));
    connect(m_sqlHistoryAnalysisObj, SIGNAL(loadOver(TSheetData)), this, SLOT(rcvSheetData(TSheetData)));
}

HistoryAnalysisForm::~HistoryAnalysisForm()
{
    delete ui;
}

void HistoryAnalysisForm::on_pushButton_clicked()
{
    if (ui->widget_condition->isVisible())
    {
        ui->widget_condition->hide();
    }
    else
    {
        ui->widget_condition->show();
    }
}
/*
    函数功能：生成分析数据
*/
void HistoryAnalysisForm::on_pushButton_query_clicked()
{
    TransInfo info;

    info.startTime = ui->dateTimeEdit_startTime->dateTime();
    info.endTime = ui->dateTimeEdit_endTime->dateTime();
    if (ui->lineEdit_address->text() != "")
    {
        info.u8Address = ui->lineEdit_address->text().toUInt();
    }
    info.nDecviceType = -1;
    emit onLoad(info);
    m_timeConsume.start();
    //info.nDecviceType = ui->comboBox_deviceType->currentText();
}

/*
    函数功能：
    生成表数据
*/
void HistoryAnalysisForm::rcvSheetData(TSheetData data)
{

    for (int i = 0; i <= 255; i++)
    {
        if (data.data[i][0] == -1)
        {
            continue;
        }
        for (int j = 0; j < 5; j++)
        {
            switch (j) {
                case 0:
                    if (i == 0)
                    {
                        ui->tableWidget_deviceStatisInfo->setItem(i, j, new QTableWidgetItem("所有数据"));
                    }
                    else
                    {
                        ui->tableWidget_deviceStatisInfo->setItem(i, j, new QTableWidgetItem(QString::number(i)));
                    }
                    break;
                case 1:
                    ui->tableWidget_deviceStatisInfo->setItem(i, j, new QTableWidgetItem("PR"));
                    break;
                case 2:
                    ui->tableWidget_deviceStatisInfo->setItem(i, j, new QTableWidgetItem(QString("%1/%2/%3").
                                                                                         arg(QString::number(data.data[i][0], 'g', 4)).arg(data.data[i][1]).arg(data.data[i][2])));
                    break;
                case 3:
                    ui->tableWidget_deviceStatisInfo->setItem(i, j, new QTableWidgetItem(QString("%1/%2/%3").
                                                                                         arg(QString::number(data.data[i][3], 'g', 4)).arg(data.data[i][4]).arg(data.data[i][5])));
                    break;
                case 4:
                    ui->tableWidget_deviceStatisInfo->setItem(i, j, new QTableWidgetItem(QString("%1/%2/%3").
                                                                                         arg(QString::number(data.data[i][6], 'g', 4)).arg(data.data[i][7]).arg(data.data[i][8])));
                    break;
                default:
                    break;

            }

        }

    }


    ui->lineEdit_cosumeTime->setText("操作耗时:" + QString::number(m_timeConsume.elapsed()) + "ms");

}
