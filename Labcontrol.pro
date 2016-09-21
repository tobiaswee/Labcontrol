QT       += core gui network widgets

TARGET = labcontrol
TEMPLATE = app


SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/sessiondisplay.cpp \
    src/sessionstarter.cpp \
    src/Lib/client.cpp \
    src/Lib/clienthelpnotificationserver.cpp \
    src/Lib/clientpinger.cpp \
    src/Lib/lablib.cpp \
    src/Lib/netstatagent.cpp \
    src/Lib/receipts_handler.cpp \
    src/Lib/receiptsprinter.cpp \
    src/Lib/session.cpp \
    src/Lib/sessionsmodel.cpp \
    src/Lib/ztree.cpp

HEADERS  += src/mainwindow.h \
    src/sessiondisplay.h \
    src/sessionstarter.h \
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
    src/Lib/ztree.h

FORMS    += src/mainwindow.ui \
    src/sessiondisplay.ui \
    src/sessionstarter.ui

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
    data/scripts/start_zTree_labcontrol2.sh \
    doc/manual.tex
