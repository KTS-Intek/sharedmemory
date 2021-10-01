# Version=0.0.1
#
# Dependencies
# sharedmemory
#

INCLUDEPATH  += $$PWD\
                $$PWD/../../defines/defines
                $$PWD/../../defines/define-types


HEADERS += \
    $$PWD/src/shared/connectiontableinsharedmemory.h \
    $$PWD/src/shared/connectiontableinsharedmemorytypes.h \
    $$PWD/src/shared/sharedmemohelper.h \
    $$PWD/src/shared/sharedmemoprotocolhelper.h \
    $$PWD/src/shared/sharedmemowritelater.h \
    $$PWD/src/shared/sharedmemowriter.h

SOURCES += \
    $$PWD/src/shared/connectiontableinsharedmemory.cpp \
    $$PWD/src/shared/sharedmemohelper.cpp \
    $$PWD/src/shared/sharedmemoprotocolhelper.cpp \
    $$PWD/src/shared/sharedmemowritelater.cpp \
    $$PWD/src/shared/sharedmemowriter.cpp


