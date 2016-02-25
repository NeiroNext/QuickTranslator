#include <qtsingleapplication/qtsingleapplication.h>
#include "widget.h"
#include <QDebug>

#define VERSION "1.2.0"


int main(int argc, char *argv[])
{    
    QtSingleApplication app(argc, argv);

    QtSingleApplication::setQuitOnLastWindowClosed(false);
    QtSingleApplication::setApplicationVersion(VERSION);

    if(app.isRunning() && !app.arguments().contains("--restart"))
        return !app.sendMessage("Something");

    Widget w;
    app.setActivationWindow(&w);

    // Show or not show
    if(!app.arguments().contains("--hide"))
        w.show();


    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &w, SLOT(show()));

    w.checkUpdates();

   return app.exec();
}


/****************************************************
   Interesting, i don't know this befor :)
   QApplication::setQuitOnLastWindowClosed(false);
****************************************************/
