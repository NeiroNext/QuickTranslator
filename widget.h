#ifndef WIDGET_H
#define WIDGET_H

#include <QMessageBox>
#include <QDir>
#include <QWidget>
#include <QDesktopWidget>
#include <QMainWindow>
#include <QProcess>
#include <QxtGlobalShortcut>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QModelIndex>
#include <QPair>
#include <QListWidget>
#include <QProgressBar>


#include "translate.h"
#include "box.h"
#include "textfield.h"
#include "smarttranslate.h"
#include "grablineedit.h"
#include "settings.h"
#include "autorun.h"
#include "crossplatform.h"


namespace Ui {
    class Widget;
}


class Widget : public QMainWindow
{
   Q_OBJECT
   
public:
   explicit Widget(QMainWindow *parent = 0);
   ~Widget();

    void setFromLanguage(QString str);
    void setToLanguage(QString str);
    void langListInit(QString url, bool initList = true);
    void appLngChange(QString lng);
    void resizeEvent(QResizeEvent *ev);
    void moveEvent(QMoveEvent *ev);
    void timerEvent(QTimerEvent *ev);

    QStringList      themeTextColor;
    QString          getTTColor(); // getThemeTextColor

    // Translate window types
    const static int TW_DEFAULT  = 0; // default
    const static int TW_CURSOR   = 1; // window at the cursor
    const static int TW_NOTIFIER = 2; // at the tray notifier


    int translateWindowType;
   

   
private:
   void trayMenuInit();
   void langListInit();
   void themesInit();

   void geometrySaveEvent();
   void nextTranslateDate(QString str);
   void appLanguageInit();

   Ui::Widget        *ui;
   QProcess          process;
   QProgressBar      *pb;
   QxtGlobalShortcut hotkey,
                     hotkeyField,
                     hotkeySmart;
   QSystemTrayIcon   *trayIcon;
   QMenu             *trayMenu;
   QList<QAction*>   trayActions;
   QPair<QStringList,QStringList>   lngs;
   QPair<QStringList,QStringList>   themeNames;
   QString           fromLang,
                     toLang;
   QStringList       appLanguages;
   int               lastFromListIndex,
                     lastToListIndex;

   Translate         *trans;
   Box               *box;
   TextField         *textfield;
   SmartTranslate    *smarttranslate;
   GrabLineEdit      *lineEdit;
   GrabLineEdit      *lineEditField;
   GrabLineEdit      *lineEditSmart;
   Settings          *settings;
   Autorun           *autorun;

   QBasicTimer       *gsTimer;          // GeometrySaveTimer
   QBasicTimer       *shTimer;          // ShowHideTimer
   int               gsTimerId,
                     shTimerId;

   int               hideOptionsHeight;
   int               showStep;

   bool              smartMode;



signals:
   void nextTranslateDataSignal(QString str);



public slots:
   void startProcess();
   void startSmartTranslating();
   void finishSmartTranslating();
   void errorProcess(QProcess::ProcessError err);
   void getSelected();
   void trayMenuSlot(QAction* a);
   void changeTheme(QString name);
   void setFromLanguage(QModelIndex index);
   void setToLanguage(QModelIndex index);
   void changeHotkey(GrabLineEdit *gle, QString key);
   void changeHotkey(QString gle, QString key);
   void changeAutorun(bool status);
   void showTranslate(QString str);
   void changeInfoType(int index);
   void showHideOptions();
   void translateText(QString str);
   void smartTranslateCount(int count);
   void applicationLanguageChange(int index);

};

#endif // WIDGET_H
