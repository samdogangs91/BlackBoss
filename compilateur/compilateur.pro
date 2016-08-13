QT += core
QT -= gui

CONFIG += c++11

TARGET = compilateur
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Instruction.cpp \
    vargen.cpp \
    type.cpp \
    Database.cpp \
    attribut.cpp \
    dbvar.cpp

HEADERS += \
    Instruction.h \
    vargen.h \
    type.h \
    Database.h \
    attribut.h \
    dbvar.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lmysqlclient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lmysqlclient
else:unix: LIBS += -L$$PWD/./ -lmysqlclient

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
