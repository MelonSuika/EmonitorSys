#ifndef LOADQSS_H
#define LOADQSS_H
#include "common.h"

class LoadQss
{
public:
    LoadQss();

public:
    static void loadQss(const QString &path)
    {
        QFile qss(path);
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }


};

#endif // LOADQSS_H
