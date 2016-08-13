QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BlackBoss_client
TEMPLATE = app

SOURCES += main.cpp \
    mainwindow.cpp \
    fenclient.cpp \
    fenconnexion.cpp

HEADERS += \
    mainwindow.h \
    fenclient.h \
    fenconnexion.h

FORMS += \
    connexion.ui \
    FenClient.ui \
    fenconnexion.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lmysqlclient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lmysqlclient
else:unix: LIBS += -L$$PWD/./ -lmysqlclient

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
