#include "crossplatform.h"

#ifdef Q_OS_LINUX
#include <QX11Info>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#endif
#ifdef Q_OS_WIN
#include <QTextCodec>
#include <windows.h>
#endif





Crossplatform::Crossplatform()
{

}





// WindowOnTopFrameIconHide constant
Qt::WindowFlags Crossplatform::_WindowOnTopFrameIconHide(){
#ifdef Q_OS_LINUX
    // Qt::SplashScreen for hide icon from the Unity panel | only for Unity
    return Qt::WindowStaysOnTopHint | Qt::SplashScreen;
#endif

#ifdef Q_OS_WIN
    return Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool;
#endif
}





// WindowCloseButtonHint constant
Qt::WindowFlags Crossplatform::_WindowCloseButtonHint(){
#ifdef Q_OS_LINUX
    //return Qt::Tool;
    return Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint;
#endif

#ifdef Q_OS_WIN
    //return Qt::WindowCloseButtonHint;
    // Only for some WindoUsers =)
    return Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint;
#endif
}





// GetSelectedProcessName constant
QString Crossplatform::_GetSelectedProcessName(){
#ifdef Q_OS_LINUX
//    return "./unix_xsel";                   // For Release
    return "./unix_xsel/unix_xsel";         // For Debug
#endif

#ifdef Q_OS_WIN
    return "win_xsel/debug/win_xsel.exe";   // For Debug
//    return "win_xsel.exe";                  // For Release
#endif
}





// Hotkey display Meta key constant
QString Crossplatform::_MetaKeyName() {
#ifdef Q_OS_LINUX
    return "Super";
#endif

#ifdef Q_OS_WIN
    return "Win";
#endif
}





// File Path prefix file:///
QString Crossplatform::_FilePathPrefix() {
#ifdef Q_OS_LINUX
    return "file://";
#endif
#ifdef Q_OS_WIN
    return "file:///";
#endif
}





//====================================// FUNCTIONS //======================================//





// setFocus for window
void Crossplatform::setFocus(QWidget *window){
#ifdef Q_OS_LINUX
    static Atom NET_ACTIVE_WINDOW = XInternAtom(QX11Info::display(), "_NET_ACTIVE_WINDOW", False);

    XClientMessageEvent xev;

    xev.type         = ClientMessage;
    xev.window       = window->winId();
    xev.message_type = NET_ACTIVE_WINDOW;
    xev.format       = 32;
    xev.data.l[0]    = 2;
    xev.data.l[1]    = CurrentTime;
    xev.data.l[2]    = 0;
    xev.data.l[3]    = 0;
    xev.data.l[4]    = 0;

    XSendEvent(QX11Info::display(), QX11Info::appRootWindow(), False,
              (SubstructureNotifyMask | SubstructureRedirectMask), (XEvent *)&xev);
#endif
#ifdef Q_OS_WIN
    SetForegroundWindow(window->winId());
#endif

}





// Return clipboard in needed encoding
QString Crossplatform::clipboard(){
    QString ret = "";
#ifdef Q_OS_WIN
    QTextCodec *tc = QTextCodec::codecForName("Windows-1251");
    QByteArray ba;
    ba.append(QApplication::clipboard()->mimeData()->data("text/html"));
    ret = tc->toUnicode(ba);
#endif
#ifdef Q_OS_LINUX
    ret = QString::fromUtf8(QApplication::clipboard()->mimeData()->data("text/html"));
#endif

    return ret;
}
