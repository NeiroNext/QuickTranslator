#include "defaulttranslator.h"
#include "ui_defaulttranslator.h"
#include "widget.h"
#include <QDebug>



DefaultTranslator::DefaultTranslator(QWidget *parent, QWidget *options, Widget *wgt, Translate *trans) :
    QWidget(0),
    ui(new Ui::DefaultTranslator)
{
    ui->setupUi(this);
    parent->layout()->addWidget(this);

    this->wgt     = wgt;
    this->trans   = trans;
    this->options = options;
    this->fromLng = ui->cbFrom;
    this->toLng   = ui->cbTo;
    this->btnHelp = ui->btnHelp;
    this->btnAbout= ui->btnAbout;

    autoTranslate = new QBasicTimer();
    transparentWgt= new QWidget(this);

    transparentWgt->setStyleSheet("background-color: black");
    transparentWgt->setGeometry(0, 0, this->width(), this->height());
    transparentWgt->hide();

    blur     = new QGraphicsBlurEffect(this);
    optWgtPA = new QPropertyAnimation(options, "pos");
    trnWgtPA = new QPropertyAnimation(transparentWgt, "windowOpacity");
    defWgtPA = new QPropertyAnimation(blur, "blurRadius");

    connect(ui->btnOptions, SIGNAL(clicked(bool)),       SLOT(toggleOptionsShow(bool)));
    connect(ui->cbFrom,     SIGNAL(activated(int)), wgt, SLOT(setFromLanguage(int)));
    connect(ui->cbTo,       SIGNAL(activated(int)), wgt, SLOT(setToLanguage(int)));
    connect(ui->translate,  SIGNAL(clicked(bool)),       SLOT(translateBtnClick()));
    connect(ui->tbFrom,     SIGNAL(textChanged()),       SLOT(textChange()));
    connect(ui->reverse,    SIGNAL(clicked(bool)),  wgt, SLOT(languageReverse()));

    toggleOptionsShow(false);

}





DefaultTranslator::~DefaultTranslator() {
    delete ui;
}





void DefaultTranslator::toggleOptionsShow(bool arg) {

    if (arg) {  // Show options
        disconnect(optWgtPA, SIGNAL(finished()), options, SLOT(hide()));
        ui->btnOptions->setChecked(false);

        transparentWgt->show();
        options->show();

        optWgtPA->setDuration(1000);
        optWgtPA->setStartValue(QPoint(width(), 0));
        optWgtPA->setEndValue(QPoint(width() - options->width(), 0));
        optWgtPA->start();

    } else {    // Hide options
        connect(optWgtPA, SIGNAL(finished()), options, SLOT(hide()));

        if(optWgtPA->state() == QPropertyAnimation::Running)
            optWgtPA->stop();
        optWgtPA->setDuration(1000);
        optWgtPA->setEndValue(QPoint(width(), 0));
        optWgtPA->setStartValue(QPoint(width() - options->width(), 0));
        optWgtPA->start();

        transparentWgt->hide();
    }

}





void DefaultTranslator::setItemsHeights(QFrame *frHeader, QFrame *frFooter1, QFrame *frFooter2) {
    ui->header->setMinimumHeight(frHeader->height());
    int height = frFooter1->minimumSizeHint().height() + frFooter2->minimumSizeHint().height();
    ui->footer->setMinimumHeight(height - 0);
    ui->footer->setMaximumHeight(height - 0);
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





// Translate Button Click
void DefaultTranslator::translateBtnClick() {
    QByteArray txt = ui->tbFrom->toPlainText().toUtf8();
    if(txt.length() > 0) {
        trans->setSimilarWords(false);
        trans->setData(wgt->fromLang, wgt->toLang, txt, this);
    }
}





// Show translate text
void DefaultTranslator::getTranslate(QString translate, QString origin, QString autoLng) {
    trans->setSimilarWords(wgt->similarWords);
    ui->tbTo->setPlainText(translate);

    setAutoLang(autoLng);
}





// Text change slot
void DefaultTranslator::textChange() {
    if(autoTranslate->isActive()){
        autoTranslate->stop();
    }
    autoTranslate->start(500, this);
    autoTranslateId = autoTranslate->timerId();
}





// TimerEvent method
void DefaultTranslator::timerEvent(QTimerEvent *ev) {
    if(ev->timerId() == autoTranslateId) {
        emit ui->translate->click();
        autoTranslate->stop();
    }
}





// Mouse press event
void DefaultTranslator::mousePressEvent(QMouseEvent *ev) {
    toggleOptionsShow(false);
}





// Resize event
void DefaultTranslator::resizeEvent(QResizeEvent *ev) {
     transparentWgt->setGeometry(0, 0, this->width(), this->height());
}





// Set autoLang QLabel value
void DefaultTranslator::setAutoLang(QString lng) {
    wgt->autoLang = lng;
    int index = wgt->lngs.first.indexOf(lng);
    if(index != -1)
        ui->autoLng->setText(wgt->lngs.second[index]);
    else
        wgt->autoLang = "";
}
