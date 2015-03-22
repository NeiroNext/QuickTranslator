#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include "widget.h"
#include "ui_widget.h"
#include <QScreen>


// Constructor
Widget::Widget(QMainWindow *parent) :
   QMainWindow(parent),
   ui(new Ui::Widget)
{
   settings = new Settings(this);
   settings->applicationLanguageChange();

   themeTextColor << "#4c4c4c" << "#4c4c4c" << "#4c4c4c";
   translateWindowType = TW_DEFAULT;

   ui->setupUi(this);

   autorun  = new Autorun();
   box      = new Box();
   trans    = new Translate(this);
   lineEdit = new GrabLineEdit();
#ifdef Q_OS_UNIX
   setWindowFlags(Qt::Tool);
#endif
#ifdef Q_OS_WIN
   setWindowFlags(Qt::WindowCloseButtonHint);
#endif
   move(200, 200);

   // Init some objects
   trayMenuInit();
   langListInit();
   themesInit();
   settings->Init();

   // Delete default QLineEdit (used for fine design only)
   delete ui->hotkey_le;
   ui->verticalLayout_2->addWidget(lineEdit);

   // Connects
   connect(&hotkey,       SIGNAL(activated()),                SLOT(startProcess()));
   connect(&process,      SIGNAL(error(QProcess::ProcessError)), SLOT(errorProcess(QProcess::ProcessError)));
   connect(&process,      SIGNAL(readyReadStandardOutput()),  SLOT(getSelected()));
   connect(trayMenu,      SIGNAL(triggered(QAction*)),        SLOT(trayMenuSlot(QAction*)));
 //connect(trans,         SIGNAL(showTranslate(QString)), box,SLOT(showTranslate(QString)));
   connect(trans,         SIGNAL(showTranslate(QString)),     SLOT(showTranslate(QString)));
   connect(ui->theme_cb,  SIGNAL(activated(QString)),         SLOT(changeTheme(QString)));
   connect(ui->from_list, SIGNAL(doubleClicked(QModelIndex)), SLOT(setFromLanguage(QModelIndex)));
   connect(ui->to_list,   SIGNAL(doubleClicked(QModelIndex)), SLOT(setToLanguage(QModelIndex)));
   connect(lineEdit,      SIGNAL(hotkeyChanged(QString)),     SLOT(changeHotkey(QString)));
   connect(ui->autorun_cb,SIGNAL(toggled(bool)),              SLOT(changeAutorun(bool)));
   connect(ui->infoWin_ch,SIGNAL(activated(int)),             SLOT(changeInfoType(int)));
}





// Destructor
Widget::~Widget(){
   delete ui;
   delete box;
   delete trans;
   delete settings;
   delete autorun;
}





// Start "get selected text" process
void Widget::startProcess(){
#ifdef Q_OS_UNIX
   //process.start(qApp->applicationDirPath()+"/xsel");
    process.start("xsel");
#endif
#ifdef Q_OS_WIN
   process.start("xsel.exe");
#endif
}





// Process error
void Widget::errorProcess(QProcess::ProcessError err){
    qDebug() << "Error run process \"xsel\"\nMessage: " << process.errorString();
    if(err == QProcess::NotOpen){
        QMessageBox::critical(this, tr("Critical error!"),
        "<font color="+getTTColor()+">"+
            tr("Unable to run the utility <b>xsel</b>, maybe it is not exists."
               "<br>The program work is not possible without it!<br>"
               "Try reinstalling the program to its normal work!")+
        "</font>");
    }
}





// Get selected text (process return data) and Translate it
void Widget::getSelected(){
   QByteArray selStr = process.readAll();
   process.close();
   trans->setData(fromLang, toLang, selStr);
}





// Init tray menu
void Widget::trayMenuInit(){
    trayIcon = new QSystemTrayIcon();
    trayIcon->setIcon(QIcon(":/files/imgs/icon-color.svg"));
    trayIcon->show();

    trayActions.append(new QAction(tr("Show/Hide"), 0));
    trayActions.append(new QAction(tr("About"), 0));
    trayActions.append(new QAction(tr("Exit"), 0));

    trayMenu = new QMenu();
    trayMenu->addActions(trayActions);
    trayMenu->insertSeparator(trayActions.last());

    trayIcon->setContextMenu(trayMenu);
}





// Init languages list
void Widget::langListInit(){
    langListInit(":/langs/lngList.json");
}


// Init languages list
void Widget::langListInit(QString url, bool initList){
    QFile fl(url);
    fl.open(QFile::ReadOnly);
    QString str = QString(fl.readAll());
    fl.close();

    if(lngs.first.count() == 0){
        QVariantList vl = QxtJSON::parse(str).value<QVariantList>();
        foreach (QVariant v, vl) {
            QVariantMap vm = v.value<QVariantMap>();
            QString mini = vm["mini"].toString();
            QString full = vm["full"].toString();
            lngs.first.append(mini);
            lngs.second.append(QString::fromUtf8(full.toAscii()));
        }
    }
    if(initList){
        ui->from_list->addItems(lngs.second);
        ui->to_list->addItems(lngs.second);

        delete ui->to_list->takeItem(0);
    }

    lastFromListIndex = 0;
    lastToListIndex   = 0;
}





// Init theme list
void Widget::themesInit(){
    QString themesUrl = QCoreApplication::applicationDirPath() + "/themes/";
    QStringList list  = QDir(themesUrl).entryList(QStringList() << "*.qss");
    QString tmp;

    themeNames.first.clear();
    themeNames.second.clear();


    foreach (QString elem, list){
        int end = elem.indexOf("{");
            end = (end != -1) ? end : elem.indexOf(".qss");
            tmp = elem.mid(0, end);
        themeNames.first.append(tmp);
        if(end != elem.indexOf(".qss"))
            tmp = elem.mid(end+1, elem.indexOf("}")-end-1);
        else
            tmp = "color";
        themeNames.second.append(tmp);
    }
    if(!themeNames.first.contains("Default")){
        themeNames.first.insert(0, "Default");
        themeNames.second.insert(0, "color");
    }

    ui->theme_cb->addItems(themeNames.first);
}





// TrayMenu slot
void Widget::trayMenuSlot(QAction *act){
    if(act == trayActions[0]){                                         // Show/Hide
        if(isHidden())
           show();
        else
           hide();
    }

    if(act == trayActions[1]){                                         // About
        QMessageBox::about(this, tr("About"), "<font color="+getTTColor()+">"+
        tr("<center><h2>Quick Translator</h2></center><br>"
           "This is simple program designed to quickly translate "
           "selected text from an unknown language to yours.<br>"
           "Features:"
           "<ul>"
           "<li> quick translation of selected text, a combination of keys that can be changed;</li>"
           "<li> display similar words from the translated;</li>"
           "<li> translate not only words but also phrases;</li>"
           "<li> translation into 80 languages.</li>"
           "</ul>"
           "The program is absolutely free and allowed to free distribution.<br><br>"
           "Author: <a href='http://vk.com/rozshko'>Mihail Rozshko</a><br>"
           "Email: <a href='mailto:mihail.rozshko@gmail.com'>mihail.rozshko@gmail.com</a><br>"
           "Site: <a href='http://sovasoft.zz.vc'>SovaSoft.zz.vc</a><br><br>"
           "Copyright &copy; SovaSoft 2014-2015")
           +"</font>");
    }
    if(act == trayActions[2]){                                        // Exit
        qApp->quit();
    }

}





// Show translate slot
void Widget::showTranslate(QString str){
    switch(translateWindowType){
        case TW_DEFAULT:
            box->setFly(false);
            emit box->showTranslate(str);
            break;
        case TW_NOTIFIER:
            box->setHidden(true);
            trayIcon->showMessage(tr("Translate"), str, QSystemTrayIcon::NoIcon, 5000);
            break;
        case TW_CURSOR:
            box->setFly(true);
            QPoint wp = QCursor::pos() + QPoint(16, 16);
            box->move(wp);
            box->showTranslate(str);
            break;
    }
}





// Set fromTranslate list item
void Widget::setFromLanguage(QModelIndex i){
    ui->from_list->item(lastFromListIndex)->setFont(QFont());
    ui->from_list->item(lastFromListIndex)->setBackground(QBrush());

    fromLang  = ui->from_list->item(i.row())->text();
    int index = lngs.second.indexOf(fromLang);
    fromLang  = lngs.first[index];

    ui->from_list->item(i.row())->setFont(QFont(NULL, -1, 700));
    ui->from_list->item(i.row())->setBackground(QBrush(QColor(128, 128, 128, 100)));

    lastFromListIndex = i.row();

    settings->Update(settings->LANG_FROM, fromLang);
}





// Set fromTranslate list item
void Widget::setFromLanguage(QString str){
    int index = lngs.first.indexOf(str);
    index = (index < 0) ? 0 : index;

    QModelIndex mi = ui->from_list->indexAt(QPoint(0, 0));
    setFromLanguage(mi.model()->index(index, 0));
}





// Set toTranslate list item
void Widget::setToLanguage(QModelIndex i){
    ui->to_list->item(lastToListIndex)->setFont(QFont());
    ui->to_list->item(lastToListIndex)->setBackground(QBrush());

    toLang    = ui->to_list->item(i.row())->text();
    int index = lngs.second.indexOf(toLang);
    toLang    = lngs.first[index];

    ui->to_list->item(i.row())->setFont(QFont(NULL, -1, 700));
    ui->to_list->item(i.row())->setBackground(QBrush(QColor(128, 128, 128, 100)));

    lastToListIndex = i.row();

    settings->Update(settings->LANG_TO, toLang);
}





// Set toTranslate list item
void Widget::setToLanguage(QString str){
    int index = lngs.first.indexOf(str) - 1; // -1 beacouse toList don't have "Detect language" item
    index = (index < 0) ? 0 : index;

    QModelIndex mi = ui->to_list->indexAt(QPoint(0, 0));
    setToLanguage(mi.model()->index(index, 0));
}





// Change application theme
void Widget::changeTheme(QString thName){
    int index = themeNames.first.indexOf(thName);

    QString name = thName;
    if(index == -1) {
        index = 0;
        name  = "Default";
    }
    QString themeName = name;
    QString iName     = themeNames.second[index];
    QString themesUrl = QCoreApplication::applicationDirPath() + "/themes/";
    name += "{" + iName + "}.qss";

    ui->theme_cb->setCurrentIndex(index);
    trayIcon->setIcon(QIcon(":/files/imgs/icon-" + iName + ".svg"));
    QFile f(themesUrl + name);
    if(!f.exists())
        f.setFileName(themesUrl + themeName + ".qss");
    if(!f.exists())
        f.setFileName(":/files/themes/" + name);
    f.open(QIODevice::ReadOnly);
    qApp->setStyleSheet(f.readAll());
    f.close();

    settings->Update(settings->APP_THEME, themeName);
}





// Change hotkey
void Widget::changeHotkey(QString key){
    hotkey.setDisabled();
    lineEdit->setText(key);

    if(!hotkey.setShortcut(QKeySequence(key))){
        lineEdit->setText(lineEdit->text()+" "+tr("Change hotkey!"));
        lineEdit->setStyleSheet("QLineEdit{color: red}");
    } else {
        hotkey.setEnabled();
        lineEdit->setStyleSheet("QLineEdit{color: black}");
    }

    settings->Update(settings->APP_HOTKEY, key);
}





// Change application autorun status
void Widget::changeAutorun(bool status){
    if(autorun->setAutorun(status)){
        ui->autorun_cb->setChecked(status);
        settings->Update(settings->APP_AUTORUN, status);
    } else {
        qDebug() << "Autorun Error!";
    }
}





// Main application's window resize
void Widget::resizeEvent(QResizeEvent *ev){
    settings->Update(settings->APP_GEOMETRY, geometry());
}





// Main application's window move
void Widget::moveEvent(QMoveEvent *ev){
    settings->Update(settings->APP_GEOMETRY, geometry());
}





// Return dialog text color for current theme
QString Widget::getTTColor(){
    int index = ui->theme_cb->currentIndex();
    return themeTextColor[index];
}





// Change info window type
void Widget::changeInfoType(int index){
    translateWindowType = index;
    ui->infoWin_ch->setCurrentIndex(index);
    settings->Update(settings->APP_INFOWINTYPE, index);
}
