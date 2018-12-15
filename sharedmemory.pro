QT       += core

linux-beagleboard-g++:{
   QT -= gui
}


INCLUDEPATH  += $$PWD\
                $$PWD/../../defines/defines

HEADERS += \
    $$PWD/src/shared/sharedmemohelper.h \
    $$PWD/src/shared/sharedmemoprotocolhelper.h \
    $$PWD/src/shared/sharedmemowritelater.h \
    $$PWD/src/shared/sharedmemowriter.h

SOURCES += \
    $$PWD/src/shared/sharedmemohelper.cpp \
    $$PWD/src/shared/sharedmemoprotocolhelper.cpp \
    $$PWD/src/shared/sharedmemowritelater.cpp \
    $$PWD/src/shared/sharedmemowriter.cpp


