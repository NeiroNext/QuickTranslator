#ifndef AUTORUN_H
#define AUTORUN_H

#include <QCoreApplication>
#include <QObject>

class Autorun
{
public:
    Autorun();
    bool setAutorun(bool status);
#ifdef Q_OS_UNIX
    int distFinder();


    static const int KDE   = 0;
    static const int GNOME = 1;
    static const int UNITY = 2;
    static const int UNKNOW= 99;
#endif


private:
#ifdef Q_OS_UNIX
    bool isFound(QStringList data, QStringList values);
    void setAutorunFile(QString url, bool create);

    int     nowDistr;
    QString userPath;
    QString autorunFile;
#endif
#ifdef Q_OS_WIN
    void winAutorun(bool stat);
#endif
};

#endif // AUTORUN_H
