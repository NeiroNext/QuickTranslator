#include "defaulttranslator.h"
#include "ui_defaulttranslator.h"
#include "widget.h"
#include <QDebug>

DefaultTranslator::DefaultTranslator(QWidget *parent, QWidget *options, Widget *wgt) :
    QWidget(0),
    ui(new Ui::DefaultTranslator)
{
    ui->setupUi(this);
    parent->layout()->addWidget(this);

    this->wgt     = wgt;
    this->options = options;
    this->fromLng = ui->cbFrom;
    this->toLng   = ui->cbTo;

    connect(ui->btnOptions, SIGNAL(clicked(bool)), SLOT(toggleOptionsShow(bool)));
    connect(ui->cbFrom, SIGNAL(activated(int)), wgt, SLOT(setFromLanguage(int)));
    connect(ui->cbTo, SIGNAL(activated(int)), wgt, SLOT(setToLanguage(int)));

    toggleOptionsShow(false);
}





DefaultTranslator::~DefaultTranslator() {
    delete ui;
}





void DefaultTranslator::toggleOptionsShow(bool arg) {
    options->setVisible(arg);

    int needMargin = ui->header->layout()->contentsMargins().left();

    if (options->isVisible()) {
        int footerHeight       = ui->footer->height();
        int translateBtnHeight = ui->translate->height();
        int needMarginBottom   = footerHeight- translateBtnHeight;


        ui->header->layout()->setContentsMargins(QMargins(needMargin, 0, 0, 0));
        ui->footer->layout()->setContentsMargins(QMargins(needMargin, 0, 0, needMargin));
        ui->mainVLayout->setContentsMargins(QMargins(needMargin, 0, 0, 0));
    } else {
        ui->header->layout()->setContentsMargins(QMargins(needMargin, 0, needMargin, 0));
        ui->footer->layout()->setContentsMargins(QMargins(needMargin, 0, needMargin, 0));
        ui->mainVLayout->setContentsMargins(QMargins(needMargin, 0, needMargin, 0));
    }
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





void DefaultTranslator::setItemsHeights(QFrame *frHeader, QFrame *frFooter1, QFrame *frFooter2) {
    ui->header->setMinimumHeight(frHeader->height());
    int height = frFooter1->minimumSizeHint().height() + frFooter2->minimumSizeHint().height();
    ui->footer->setMinimumHeight(height - 0);
    ui->footer->setMaximumHeight(height - 0);
}





