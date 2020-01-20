#ifndef SQLCOMMON_H
#define SQLCOMMON_H
#include "common.h"
typedef struct transInfo
{
    uchar u8Address;
    int nDecviceType;
    QDateTime startTime;
    QDateTime endTime;
}TransInfo;

Q_DECLARE_METATYPE(TransInfo);

typedef struct sheetData
{
    int nDataDocumentCount;
    double data[260][9];

}TSheetData;
Q_DECLARE_METATYPE(TSheetData);



#endif // SQLCOMMON_H
