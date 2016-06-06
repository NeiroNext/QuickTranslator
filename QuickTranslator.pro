#-------------------------------------------------
# Project created by QtCreator 2014-02-23T18:56:41
#-------------------------------------------------

SUBDIRS += src

win32 {
    SUBDIRS += src/win_xsel
}
unix {
    SUBDIRS += src/unix_xsel
}

TEMPLATE =  subdirs
VERSION  =  1.2.0

