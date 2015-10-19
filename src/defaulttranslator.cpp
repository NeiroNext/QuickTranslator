#include "defaulttranslator.h"
#include "ui_defaulttranslator.h"
#include "widget.h"
#include <QDebug>

DefaultTranslator::DefaultTranslator(QWidget *parent, QWidget *options, Widget *wgt) :
    QWidget(0),
    ui(new Ui::DefaultTranslator)
{
    ui->setupUi(this);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(this);
    parent->setLayout(layout);

    this->wgt     = wgt;
    this->options = options;
    this->fromLng = ui->cbFrom;
    this->toLng   = ui->cbTo;

    connect(ui->btnOptions, SIGNAL(clicked(bool)), SLOT(toggleOptionsShow(bool)));
    connect(ui->cbFrom, SIGNAL(activated(int)), wgt, SLOT(setFromLanguage(int)));
    connect(ui->cbTo, SIGNAL(activated(int)), wgt, SLOT(setToLanguage(int)));
}





DefaultTranslator::~DefaultTranslator() {
    delete ui;
}





void DefaultTranslator::toggleOptionsShow(bool arg) {
    options->setVisible(arg);
}





void DefaultTranslator::loadLanguages(QList<QListWidgetItem*> items) {
    for(int i=0; i<items.size(); i++) {
        ui->cbFrom->addItem(items[i]->icon(), items[i]->text());
        ui->cbTo->addItem(items[i]->icon(), items[i]->text());
    }
    ui->cbTo->removeItem(0);
}

void DefaultTranslator::loadLanguages(QStringList items) {
    ui->cbFrom->addItems(items);
    ui->cbTo->addItems(items);
    ui->cbTo->removeItem(0);
}





