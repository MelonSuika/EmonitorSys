#ifndef HISTORYANALYSISFORM_H
#define HISTORYANALYSISFORM_H

#include <QWidget>
#include "sqlhistoryanalysisobject.h"
#include <QThread>
#include "sqlcommon.h"
namespace Ui {
class HistoryAnalysisForm;
}

class HistoryAnalysisForm : public QWidget
{
    Q_OBJECT

public:
    explicit HistoryAnalysisForm(QWidget *parent = nullptr);
    ~HistoryAnalysisForm();
signals:
    void onLoad(TransInfo info);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_query_clicked();

    void rcvSheetData(TSheetData);

private:
    Ui::HistoryAnalysisForm *ui;
    SqlHistoryAnalysisObject *m_sqlHistoryAnalysisObj;
    QThread *m_sqlThread;
    /* 耗时统计 */
    QTime m_timeConsume;

};

#endif // HISTORYANALYSISFORM_H
