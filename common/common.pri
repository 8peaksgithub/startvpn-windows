BUILDDIR =  $$OUT_PWD/tmp/
OBJECTS_DIR = $${BUILDDIR}
MOC_DIR = $${BUILDDIR}
RCC_DIR = $${BUILDDIR}
UI_DIR = $${BUILDDIR}

HEADERS += \
    $$PWD/common.h \
    $$PWD/config.h \
    $$PWD/crypto.h \
    $$PWD/filelogger.h \
    $$PWD/logger.h \
    $$PWD/openconnectapi.h \
    $$PWD/painter.h

SOURCES += \
    $$PWD/common.cpp \
    $$PWD/config.cpp \
    $$PWD/crypto.cpp \
    $$PWD/filelogger.cpp \
    $$PWD/logger.cpp \
    $$PWD/openconnectapi.cpp \
    $$PWD/painter.cpp
