#-------------------------------------------------
# Project created by QtCreator 2014-02-23T18:56:41
#-------------------------------------------------

QT      +=  core gui widgets network

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
             autorun.cpp        \
             textfield.cpp      \
             smarttranslate.cpp \
             crossplatform.cpp

HEADERS  +=  widget.h           \
             translate.h        \
             box.h              \
             grablineedit.h     \
             settings.h         \
             autorun.h          \
             textfield.h        \
             smarttranslate.h   \
             crossplatform.h

FORMS    +=  widget.ui          \
             box.ui             \
             textfield.ui

RESOURCES += main.qrc

unix {
    #LIBS += -Llib
    #QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/lib\''
    CONFIG  += x11
}

win32 {
    LIBS += -L"../QuickTranslator/libs/win32"

    RC_FILE = win_icon.rc
}

TRANSLATIONS += ../langs/RU_lngApp.ts    \
                ../langs/UK_lngApp.ts

OTHER_FILES +=

