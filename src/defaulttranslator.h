#ifndef DEFAULTTRANSLATOR_H
#define DEFAULTTRANSLATOR_H

#include <QWidget>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QComboBox>
#include <QTextBrowser>
#include <QFrame>
#include <QPushButton>
#include <QBasicTimer>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#include <QGraphicsOpacityEffect>
#include "translate.h"
#include "gettranslate.h"
#include "needthemechange.h"

class Widget;

namespace Ui {
    class DefaultTranslator;
}

class DefaultTranslator : public QWidget, public GetTranslate, public NeedThemeChange
{
    Q_OBJECT

public:
    explicit DefaultTranslator(QWidget *parent, QWidget *options, Widget *wgt, Translate *trans);
    ~DefaultTranslator();

    void loadLanguages(QList<QListWidgetItem*> itemsFrom);
    void loadLanguages(QStringList items);
    void setAutoLang(QString lng);

    QComboBox   *fromLng,
                *toLng;
    QPushButton *btnHelp,
                *btnAbout;

    void getTranslate(QString translate, QString origin, QString autoLng);
    void changeTheme(QString color);
    void timerEvent(QTimerEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void resizeEvent(QResizeEvent *ev);


private:
    Ui::DefaultTranslator *ui;
    QBasicTimer           *autoTranslate;
    int                    autoTranslateId;

    QWidget               *options,
                          *transparentWgt;
    QPropertyAnimation    *optWgtPA,    // options widget PropertyAnimation
                          *trnWgtPA,    // transparent widget PropertyAnimation
                          *defWgtPA;    // default widget PropertyAnimation
    QGraphicsBlurEffect   *blurGE;      // Blur effect for this widget
    QGraphicsOpacityEffect *opacityGE;  // Opacity graphics effect
    Widget                *wgt;
    Translate             *trans;


public slots:
    void toggleOptionsShow(bool arg);
    void translateBtnClick();
    void textChange();

};


#endif // DEFAULTTRANSLATOR_H
