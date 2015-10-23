#ifndef DEFAULTTRANSLATOR_H
#define DEFAULTTRANSLATOR_H

#include <QWidget>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QComboBox>
#include <QFrame>

class Widget;

namespace Ui {
    class DefaultTranslator;
}

class DefaultTranslator : public QWidget
{
    Q_OBJECT

public:
    explicit DefaultTranslator(QWidget *parent, QWidget *options, Widget *wgt);
    ~DefaultTranslator();

    void loadLanguages(QList<QListWidgetItem*> itemsFrom);
    void loadLanguages(QStringList items);
    void setItemsHeights(QFrame *frHeader, QFrame *frFooter1, QFrame *frFooter2);

    QComboBox *fromLng, *toLng;

private:
    Ui::DefaultTranslator *ui;
    QWidget *options;
    Widget  *wgt;


public slots:
    void toggleOptionsShow(bool arg);

};

#endif // DEFAULTTRANSLATOR_H
