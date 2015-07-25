#include <qtsingleapplication/qtsingleapplication.h>
#include "widget.h"
#include <QDebug>



int main(int argc, char *argv[])
{    
    QtSingleApplication app(argc, argv);
    QtSingleApplication::setQuitOnLastWindowClosed(false);

    if(app.isRunning() && app.arguments().indexOf("--restart") == -1)
        return !app.sendMessage("Something");

    Widget w;
    app.setActivationWindow(&w);

    // Show or not show
    if(app.arguments().indexOf("--hide") == -1)
        w.show();

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &w, SLOT(show()));

   return app.exec();
}


/****************************************************
   Interesting, i don't know this befor :)
   QApplication::setQuitOnLastWindowClosed(false);
****************************************************/
