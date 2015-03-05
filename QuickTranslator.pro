#-------------------------------------------------
# Project created by QtCreator 2014-02-23T18:56:41
#-------------------------------------------------

QT       +=  core gui widgets network

include(qtsingleapplication/qtsingleapplication.pri)

CONFIG  += qxt
QXT     += core gui


TARGET   =  QuickTranslator
TEMPLATE =  app



SOURCES  +=  main.cpp           \
             widget.cpp         \
             translate.cpp      \
             box.cpp            \
             grablineedit.cpp   \
             settings.cpp       \
             autorun.cpp

HEADERS  +=  widget.h           \
             translate.h        \
             box.h              \
             grablineedit.h     \
             settings.h         \
             autorun.h

FORMS    +=  widget.ui          \
             box.ui

RESOURCES += main.qrc

win32 {
    LIBS += -L"../QuickTranslator/libs/win32"   \

}

TRANSLATIONS += RU_lngApp.ts    \
                UK_lngApp.ts
