QT       += core gui
QT       +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = BlackBoss_server
TEMPLATE = app
CONFIG += console c++11


SOURCES += \
    src/Database/Database.cpp \
    src/Database/DatabaseFun.cpp \
    src/Database/DatabaseModel.cpp \
    src/Database/DatabaseMot.cpp \
    src/Database/DatabasePers.cpp \
    src/Database/DatabaseUser.cpp \
    src/Database/DatabaseVerb.cpp \
    src/Environnement/Event.cpp \
    src/Environnement/Modele.cpp \
    src/Environnement/Objet.cpp \
    src/Environnement/Personne.cpp \
    src/Environnement/User.cpp \
    src/Environnement/Variable.cpp \
    src/IA/BlackIa.cpp \
    src/IA/comprehension.cpp \
    src/IA/ExpArith.cpp \
    src/IA/fun.cpp \
    src/IA/InBlackIa.cpp \
    src/IA/Instruction.cpp \
    src/IA/Monome.cpp \
    src/IA/phrase.cpp \
    src/IA/Puissance.cpp \
    src/IA/SubExpr.cpp \
    src/IA/Sujet.cpp \
    src/Langage/Adjectif.cpp \
    src/Langage/Adverbe.cpp \
    src/Langage/Conjonction.cpp \
    src/Langage/Conjugaison.cpp \
    src/Langage/Determinant.cpp \
    src/Langage/Mot.cpp \
    src/Langage/Nom.cpp \
    src/Langage/NomPropre.cpp \
    src/Langage/Prenom.cpp \
    src/Langage/Preposition.cpp \
    src/Langage/Pronom.cpp \
    src/Langage/PronomPers.cpp \
    src/Langage/Verbe.cpp \
    src/main.cpp \
    src/Environnement/server.cpp

HEADERS += \
    include/Database/Database.h \
    include/Database/DatabaseFun.h \
    include/Database/DatabaseModel.h \
    include/Database/DatabaseMot.h \
    include/Database/DatabasePers.h \
    include/Database/DatabaseUser.h \
    include/Database/DatabaseVerb.h \
    include/Environnement/Event.h \
    include/Environnement/Modele.h \
    include/Environnement/Objet.h \
    include/Environnement/Personne.h \
    include/Environnement/User.h \
    include/Environnement/Variable.h \
    include/IA/BlackIa.h \
    include/IA/comprehension.h \
    include/IA/ExpArith.h \
    include/IA/InBlackIa.h \
    include/IA/Instruction.h \
    include/IA/Monome.h \
    include/IA/other.h \
    include/IA/phrase.h \
    include/IA/Puissance.h \
    include/IA/SubExpr.h \
    include/IA/Sujet.h \
    include/Langage/Adjectif.h \
    include/Langage/Adverbe.h \
    include/Langage/Conjonction.h \
    include/Langage/Conjugaison.h \
    include/Langage/Determinant.h \
    include/Langage/Mot.h \
    include/Langage/Nom.h \
    include/Langage/NomPropre.h \
    include/Langage/Prenom.h \
    include/Langage/Preposition.h \
    include/Langage/Pronom.h \
    include/Langage/PronomPers.h \
    include/Langage/Verbe.h \
    include/Environnement/server.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lmysqlclient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lmysqlclient
else:unix: LIBS += -L$$PWD/./ -lmysqlclient

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lmysqlclient_r
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lmysqlclient_r
else:unix: LIBS += -L$$PWD/./ -lmysqlclient_r

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lPocoFoundation
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lPocoFoundation
else:unix: LIBS += -L$$PWD/./ -lPocoFoundation

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lPocoFoundationd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lPocoFoundationd
else:unix: LIBS += -L$$PWD/./ -lPocoFoundationd

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./release/ -lPocoFoundation
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./debug/ -lPocoFoundation
else:unix: LIBS += -L$$PWD/./ -lPocoFoundation

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

FORMS += \
    serv.ui
