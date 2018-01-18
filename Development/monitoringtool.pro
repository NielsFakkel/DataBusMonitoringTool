QT += \
    widgets serialport \
    sql

RC_FILE = datamonitor.rc

TARGET = databusmonitor
TEMPLATE = app

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp \
    console.cpp

HEADERS += \
    mainwindow.h \
    settingsdialog.h \
    console.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    console.ui

RESOURCES += \
    monitoringtool.qrc

target.path = C:\Users\Niels\Documents\GITS\databusmonitor
INSTALLS += target

DISTFILES += \
    tudelft.ico \
    datamonitor.rc

QMAKE_CXXFLAGS += \
    -static-libgcc \
    -static-libstdc++
