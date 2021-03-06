TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

copydata.commands = $(COPY_DIR) -r $$PWD/Data $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

SOURCES += main.cpp \
    bmp_load.cpp

HEADERS += \
    bmp_load.h


LIBS += -lX11
LIBS += -lXi
LIBS += -lXmu
LIBS += -lglut
LIBS += -lGL
LIBS += -lGLU
LIBS += -lm

DISTFILES += \
    Data/lesson6/NeHe.bmp
