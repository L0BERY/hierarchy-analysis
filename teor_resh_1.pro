QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    calculate.cpp \
    db.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    calculate.h \
    db.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../Program Files/MySQL/MySQL Server 8.0/lib/' -llibmysql
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../Program Files/MySQL/MySQL Server 8.0/lib/' -llibmysqld
else:unix: LIBS += -L$$PWD/'../../../Program Files/MySQL/MySQL Server 8.0/lib/' -llibmysql

INCLUDEPATH += $$PWD/'../../../Program Files/MySQL/MySQL Server 8.0/include'
DEPENDPATH += $$PWD/'../../../Program Files/MySQL/MySQL Server 8.0/include'

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../Program Files/MySQL/MySQL Server 8.0/lib/' -lmysqlclient
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../Program Files/MySQL/MySQL Server 8.0/lib/' -lmysqlclientd
else:unix: LIBS += -L$$PWD/'../../../Program Files/MySQL/MySQL Server 8.0/lib/' -lmysqlclient

INCLUDEPATH += $$PWD/'../../../Program Files/MySQL/MySQL Server 8.0/include'
DEPENDPATH += $$PWD/'../../../Program Files/MySQL/MySQL Server 8.0/include'
