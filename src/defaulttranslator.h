#ifndef DEFAULTTRANSLATOR_H
#define DEFAULTTRANSLATOR_H

#include <QWidget>
#include <QListWidgetItem>
#include <QMainWindow>
#include <QComboBox>

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

    QComboBox *fromLng, *toLng;

private:
    Ui::DefaultTranslator *ui;
    QWidget *options;
    Widget  *wgt;


public slots:
    void toggleOptionsShow(bool arg);

};

#endif // DEFAULTTRANSLATOR_H
