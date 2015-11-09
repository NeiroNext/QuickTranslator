#ifndef DEFAULTTRANSLATOR_H
#define DEFAULTTRANSLATOR_H

#include <QWidget>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QComboBox>
#include <QTextBrowser>
#include <QFrame>
#include <QBasicTimer>
#include "translate.h"
#include "gettranslate.h"

class Widget;

namespace Ui {
    class DefaultTranslator;
}

class DefaultTranslator : public QWidget, public GetTranslate
{
    Q_OBJECT

public:
    explicit DefaultTranslator(QWidget *parent, QWidget *options, Widget *wgt, Translate *trans);
    ~DefaultTranslator();

    void loadLanguages(QList<QListWidgetItem*> itemsFrom);
    void loadLanguages(QStringList items);
    void setItemsHeights(QFrame *frHeader, QFrame *frFooter1, QFrame *frFooter2);

    QComboBox *fromLng, *toLng;

    void getTranslate(QString translate, QString origin, QString autoLng);
    void timerEvent(QTimerEvent *ev);


private:
    Ui::DefaultTranslator *ui;
    QBasicTimer           *autoTranslate;
    int                    autoTranslateId;

    QWidget               *options;
    Widget                *wgt;
    Translate             *trans;


public slots:
    void toggleOptionsShow(bool arg);
    void translateBtnClick();
    void textChange();

};


#endif // DEFAULTTRANSLATOR_H
