#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QRect>
#include <QTranslator>



class Widget;

class Settings : public QObject
{
    Q_OBJECT


public:
    explicit Settings(Widget *parent = 0);
    ~Settings();
    void Init();
    void Load();
    void Update(QString str, QVariant val);
    void applicationLanguageChange();

    QString LANG_FROM;
    QString LANG_TO;
    QString APP_THEME;
    QString APP_AUTORUN;
    QString APP_GEOMETRY;
    QString APP_INFOWINTYPE;
    QString APP_LANG;
    QString LASTLIST_FROM;
    QString LASTLIST_TO;
    QString HOTKEY_MAIN;
    QString HOTKEY_FIELD;
    QString HOTKEY_SMART;
    QString TRANS_CLIPBOARD;
    QString TRANS_SIMILAR;


private:
    QString   getUserLang();

    Widget    *w;
    QSettings *s;
    QString   from,
              to,
              hotkey,
              hotkeyField,
              hotkeySmart,
              themeName,
              appLang;
    bool      isAutorun;
    bool      copyToClipboard;
    bool      similarWords;
    QRect     geometry;
    int       infowintype;
    QTranslator *translator;



signals:

public slots:

};

#endif // SETTINGS_H
