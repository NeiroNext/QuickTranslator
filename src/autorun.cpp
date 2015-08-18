#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QSettings>
#include "autorun.h"


// Constructor
Autorun::Autorun(){
#ifdef Q_OS_UNIX
    userPath = QDir::homePath();
    nowDistr = distFinder();

    autorunFile =   "[Desktop Entry]\n"
                    "Exec="+QCoreApplication::applicationFilePath()+" --hide\n"
                    "Name="+QCoreApplication::applicationName()+"\n"
                    "Type=Application\n"
                    "X-KDE-StartupNotify=false\n"
                    "X-GNOME-Autostart-enabled=true";
#endif
}





// Set application autorun
bool Autorun::setAutorun(bool status){
#ifdef Q_OS_UNIX
    switch(nowDistr){


    case GNOME:
    case UNITY:
        if(!QDir(userPath + "/.config/autostart").exists())
            QDir(userPath + "/.config").mkdir("autostart");
        setAutorunFile(userPath+"/.config/autostart", status);
        return true;

    case KDE:
        if(!QDir(userPath + "/.kde/Autostart").exists())
            QDir(userPath + "/.kde").mkdir("Autostart");
        setAutorunFile(userPath+"/.kde/Autostart", status);
        return true;



    default:
        return false;
    }

#endif // Q_OS_LINUX


#ifdef Q_OS_WIN
    winAutorun(status);
#endif // Q_O_WIN

    return true;
}





#ifdef Q_OS_UNIX
// Find linux distributive
int Autorun::distFinder(){
    QDir proc("/proc/");
    QStringList ls = proc.entryList(QDir::Dirs, QDir::Name);

    ls.removeOne(".");
    ls.removeOne("..");

    int index = ls.indexOf(QRegExp("[a-zA-Z]{1,}"));
    int len   = ls.size() - index;
    for(int i=0; i<len; i++)
        ls.removeAt(index);

    for(int i = 0; i < ls.size(); i++){
        QFile f("/proc/"+ls[i]+"/comm");
        f.open(QIODevice::ReadOnly);

        QString name(f.readAll());
        ls[i] = name.mid(0, name.length()-1);
    }

    // Find distributive original process
    if(isFound(ls, QStringList()<<"kdeinit4"<<"kdeinit"<<"krunner"))
        return KDE;
    else if(isFound(ls, QStringList()<<"unity-panel-ser"<<"unity-files-dae"))
        return UNITY;
    else if(isFound(ls, QStringList()<<"gnome-session"))
        return GNOME;
    return UNKNOW;
}





// Found equals in the QStringList one of the values
bool Autorun::isFound(QStringList data, QStringList values){
    for(int i=0; i<values.size(); i++)
        if(data.contains(values[i]))
            return true;
    return false;
}





// Add or Remove autorun file
void Autorun::setAutorunFile(QString url, bool create){
    QFile f(url+"/QuickTranslator.desktop");

    if(f.exists()){
        if(!create)
            f.remove();
    } else {
        if(create){
            f.open(QIODevice::WriteOnly | QIODevice::Text);
            QTextStream ts(&f);
            ts << autorunFile;
            f.close();
        }
    }

}
#endif





#ifdef Q_OS_WIN
// Add or remove program from autostart in win registry
void Autorun::winAutorun(bool stat){
    QSettings *reg = new QSettings("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if(stat) {
        reg->setValue(QCoreApplication::applicationName(), QDir::toNativeSeparators(QCoreApplication::applicationFilePath() +" --hide"));
        reg->sync();
    } else {
        reg->remove(QCoreApplication::applicationName());
    }

    delete reg;
}
#endif
