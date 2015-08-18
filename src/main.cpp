#include <qtsingleapplication/qtsingleapplication.h>
#include "widget.h"
#include <QDebug>



int main(int argc, char *argv[])
{    
    QtSingleApplication app(argc, argv);
    QtSingleApplication::setQuitOnLastWindowClosed(false);

    if(app.isRunning() && !app.arguments().contains("--restart"))
        return !app.sendMessage("Something");

    Widget w;
    app.setActivationWindow(&w);

    // Show or not show
    if(!app.arguments().contains("--hide"))
        w.show();

    QObject::connect(&app, SIGNAL(messageReceived(const QString&)), &w, SLOT(show()));

   return app.exec();
}


/****************************************************
   Interesting, i don't know this befor :)
   QApplication::setQuitOnLastWindowClosed(false);
****************************************************/
