#ifndef AUTORUN_H
#define AUTORUN_H

#include <QCoreApplication>
#include <QObject>

class Autorun
{
public:
    Autorun();
    bool setAutorun(bool status);
    int distFinder();

    static const int KDE   = 0;
    static const int GNOME = 1;
    static const int UNITY = 2;
    static const int UNKNOW= 99;


private:
    bool isFound(QStringList data, QStringList values);
    void setAutorunFile(QString url, bool create);

    int     nowDistr;
    QString userPath;
    QString autorunFile;

};

#endif // AUTORUN_H
