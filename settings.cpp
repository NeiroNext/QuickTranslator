#include <QLocale>
#include <QDebug>
#include "settings.h"
#include "widget.h"


// Constructor
Settings::Settings(Widget *parent) : QObject() {
    LANG_FROM        = "language/from";
    LANG_TO          = "language/to";
    APP_THEME        = "application/theme";
    APP_AUTORUN      = "application/autorun";
    APP_GEOMETRY     = "application/geometry";
    APP_INFOWINTYPE  = "application/translate_win_type";
    LASTLIST_FROM    = "last_list/from";
    LASTLIST_TO      = "last_list/to";
    HOTKEY_MAIN      = "hotkey/main";
    HOTKEY_FIELD     = "hotkey/field";
    HOTKEY_SMART     = "hotkey/smart";

    this->w = parent;
    s = new QSettings("QuikTranslator", "config");
}





// Destructor
Settings::~Settings(){
    delete s;
    delete translator;
}





// Init application settings
void Settings::Init(){
   QString userLng = getUserLang();

   from        = s->value(LANG_FROM, "auto").toString();
   to          = s->value(LANG_TO, userLng).toString();
   themeName   = s->value(APP_THEME, "Default").toString();
   hotkey      = s->value(HOTKEY_MAIN, "Shift+Meta+Z").toString();
   hotkeyField = s->value(HOTKEY_FIELD, "Shift+Meta+T").toString();
   hotkeySmart = s->value(HOTKEY_SMART, "Shift+Meta+S").toString();
   isAutorun   = s->value(APP_AUTORUN, true).toBool();
   geometry    = s->value(APP_GEOMETRY, w->geometry()).toRect();
   infowintype = s->value(APP_INFOWINTYPE, 0).toInt();

   w->setFromLanguage(from);
   w->setToLanguage(to);
   w->changeTheme(themeName);
   w->changeHotkey(HOTKEY_MAIN,  hotkey);
   w->changeHotkey(HOTKEY_FIELD, hotkeyField);
   w->changeHotkey(HOTKEY_SMART, hotkeySmart);
   w->changeAutorun(isAutorun);
   w->setGeometry(geometry);
   w->changeInfoType(infowintype);
}





// Update application settings
void Settings::Update(QString str, QVariant val){
    s->setValue(str, val);
    s->sync();
}





// Change application language
void Settings::applicationLanguageChange(){
    QString lngUrl    = QCoreApplication::applicationDirPath() + "/langs/";
    QString langUpper = getUserLang().toUpper();

    if(langUpper != "EN"){
        if(QFile(lngUrl + langUpper + "_lngList.json").exists())
            w->langListInit(lngUrl + langUpper + "_lngList.json", false);
        if(QFile(lngUrl + langUpper + "_lngApp.qm").exists()){
            translator = new QTranslator();
            translator->load(langUpper + "_lngApp.qm", lngUrl);
            qApp->installTranslator(translator);
        }
    }

}





// Return user language
QString Settings::getUserLang(){
    QString userLocale = QLocale::system().name();
    int len            = userLocale.lastIndexOf('_');
    QString userLng    = userLocale.mid(0, len);
    if(len < 0)          userLng= "en";

    return userLng;
}


