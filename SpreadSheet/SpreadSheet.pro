#-------------------------------------------------
#
# Project created by QtCreator 2019-07-11T14:00:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SF6Esys
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

# 通用模块
INCLUDEPATH += ./include
INCLUDEPATH += $$PWD/spreadsheet
INCLUDEPATH += $$PWD/glog
INCLUDEPATH += $$PWD/qcustomplot


include     ($$PWD/qcustomplot/qcustomplot.pri)
include     ($$PWD/glog/glog.pri)
include     ($$PWD/spreadsheet/spreadsheet.pri)



# 主要的几个窗口模块分类
INCLUDEPATH += $$PWD/datasheetform
INCLUDEPATH += $$PWD/chartform
INCLUDEPATH += $$PWD/historyanalysisform

include     ($$PWD/datasheetform/datasheetform.pri)
include     ($$PWD/chartform/chartform.pri)
include     ($$PWD/historyanalysisform/historyanalysisform.pri)


SOURCES += \
    ./source/common.cpp \
    ./source/dashboardform.cpp \
    ./source/dashboardtabwidget.cpp \
        ./source/main.cpp \
        ./source/mainwindow.cpp \
    ./source/rtchartform.cpp \
    ./source/rtdatasheetform.cpp \
    ./source/serialportinfo.cpp \
    ./source/setdevicetypeform.cpp \
    ./source/deviceinfo.cpp \
    ./source/addchilddeviceform.cpp \
    ./source/callout.cpp \
    ./source/gasrelaymonitorform.cpp \
    ./source/myqquickwidget.cpp \
    ./source/rtmonitorform.cpp \
    ./source/singledashform.cpp \
    ./source/singledash.cpp \
    ./source/pagenavigator.cpp \
    ./source/aboutform.cpp \


HEADERS += \
    ./include/common.h \
        ./include/connection.h \
    ./include/dashboardform.h \
    ./include/dashboardtabwidget.h \
        ./include/mainwindow.h \
    ./include/rtchartform.h \
    ./include/rtdatasheetform.h \
    ./include/serialportinfo.h \
    ./include/setdevicetypeform.h \
    ./include/deviceinfo.h \
    ./include/addchilddeviceform.h \
    ./include/callout.h \
    ./include/gasrelaymonitorform.h \
    ./include/myqquickwidget.h \
    ./include/rtmonitorform.h \
    ./include/singledashform.h \
    ./include/singledash.h \
    ./include/pagenavigator.h \
    ./include/loadqss.h \
    ./include/aboutform.h \

UI_DIR=./ui
FORMS += \
        ./ui/dashboardform.ui \
        ./ui/dashboardtabwidget.ui \
        ./ui/mainwindow.ui \
        ./ui/rtchartform.ui \
        ./ui/rtdatasheetform.ui \
        ./ui/setdevicetypeform.ui \
        ./ui/addchilddeviceform.ui \
        ./ui/gasrelaymonitorform.ui \
        ./ui/rtmonitorform.ui \
        ./ui/singledashform.ui \
        ./ui/pagenavigator.ui \
        ./ui/aboutform.ui \

#打印支持
QT += printsupport
#使用串口
QT += serialport
#使用sql
QT += sql
#多线程
QT += concurrent
#敏捷UI
QT += quick
#图表
QT += charts
QT += quickwidgets
QT += widgets


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    dialcontrol/content/Dial.qml \
    dialcontrol/content/QuitButton.qml \
    dialcontrol/content/background.png \
    dialcontrol/content/needle.png \
    dialcontrol/content/needle_shadow.png \
    dialcontrol/content/overlay.png \
    dialcontrol/content/quit.png \
    dialcontrol/dialcontrol.qml \
    dialcontrol/doc/images/qml-dialcontrol-example.png \
    dialcontrol/doc/src/dialcontrol.qdoc \
    image/down_arrow.png \
    image/gage.png \
    image/lanso.png \
    qss/widget-black.qss \
    qss/widget-blue.qss \
    qss/widget-darkOrange.qss

RESOURCES += \
    dialcontrol/dialcontrol.qrc \
    ./qrc/mainwindow.qrc \
    ./qrc/icon.qrc

RC_ICONS += :/../image/bitbug_favicon_128.ico

win32: LIBS += -L$$PWD/lib/ -llibglog.dll

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
