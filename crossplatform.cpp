#include "crossplatform.h"

#ifdef Q_OS_LINUX
#include <QX11Info>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
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
    return Qt::Tool;
#endif

#ifdef Q_OS_WIN
    return Qt::WindowCloseButtonHint;
#endif
}





// GetSelectedProcessName constant
QString Crossplatform::_GetSelectedProcessName(){
#ifdef Q_OS_LINUX
    return "xsel";
#endif

#ifdef Q_OS_WIN
    return "xsel.exe";
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

}
