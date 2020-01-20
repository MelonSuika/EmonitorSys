#ifndef SQLHISTORYANALYSISOBJECT_H
#define SQLHISTORYANALYSISOBJECT_H

#include "sqlcommon.h"

class SqlHistoryAnalysisObject : public QObject
{
    Q_OBJECT
public:
    SqlHistoryAnalysisObject();

private slots:
    void readData(TransInfo info);
signals:
    void loadOver(TSheetData);

private:
    QSqlQuery *m_query;
};

#endif // SQLHISTORYANALYSISOBJECT_H
