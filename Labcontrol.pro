QT       += core gui network widgets

TARGET = labcontrol
TEMPLATE = app


SOURCES += src/localzleafstarter.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/manualprintingsetup.cpp \
    src/Lib/client.cpp \
    src/Lib/clienthelpnotificationserver.cpp \
    src/Lib/clientpinger.cpp \
    src/Lib/lablib.cpp \
    src/Lib/netstatagent.cpp \
    src/Lib/receipts_handler.cpp \
    src/Lib/receiptsprinter.cpp \
    src/Lib/session.cpp \
    src/Lib/sessionsmodel.cpp \
    src/Lib/settings.cpp \
    src/Lib/ztree.cpp

HEADERS  += src/localzleafstarter.h \
    src/mainwindow.h \
    src/manualprintingsetup.h \
    src/Lib/client.h \
    src/Lib/clienthelpnotificationserver.h \
    src/Lib/clientpinger.h \
    src/Lib/global.h \
    src/Lib/lablib.h \
    src/Lib/netstatagent.h \
    src/Lib/receipts_handler.h \
    src/Lib/receiptsprinter.h \
    src/Lib/session.h \
    src/Lib/sessionsmodel.h \
    src/Lib/settings.h \
    src/Lib/ztree.h

FORMS    += src/localzleafstarter.ui \
    src/mainwindow.ui \
    src/manualprintingsetup.ui

QMAKE_CXXFLAGS += -std=c++11

OTHER_FILES += \
    labcontrol.desktop \
    LICENSE \
    PROGRAMMING \
    README \
    data/example_header.tex \
    data/Labcontrol.conf \
    data/scripts/kill_zLeaf_labcontrol2.sh \
    data/scripts/start_zLeaf_labcontrol2.sh \
    doc/manual.tex

INCLUDEPATH += /usr/local/include/
