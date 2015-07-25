#ifndef CROSSPLATFORM_H
#define CROSSPLATFORM_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QClipboard>
#include <QApplication>
#include <QMimeData>





class Crossplatform
{
public:
    Crossplatform();

    // Function variables =(
    static Qt::WindowFlags _WindowOnTopFrameIconHide();
    static Qt::WindowFlags _WindowCloseButtonHint();
    static QString         _GetSelectedProcessName();

    // Normal functions
    static void    setFocus(QWidget *window);
    static QString clipboard();

};

#endif // CROSSPLATFORM_H
