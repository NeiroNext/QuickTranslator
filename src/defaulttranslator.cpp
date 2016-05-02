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

    blurGE   = new QGraphicsBlurEffect(this);
    opacityGE= new QGraphicsOpacityEffect(transparentWgt);
    optWgtPA = new QPropertyAnimation(options, "pos");
    trnWgtPA = new QPropertyAnimation(opacityGE, "opacity");
    defWgtPA = new QPropertyAnimation(blurGE, "blurRadius");

    blurGE->setBlurRadius(1);
    blurGE->setBlurHints(QGraphicsBlurEffect::AnimationHint);
    opacityGE->setOpacity(0.5);
    this->setGraphicsEffect(blurGE);
    transparentWgt->setGraphicsEffect(opacityGE);

    connect(ui->btnOptions, SIGNAL(clicked(bool)),       SLOT(toggleOptionsShow(bool)));
    connect(ui->cbFrom,     SIGNAL(activated(int)), wgt, SLOT(setFromLanguage(int)));
    connect(ui->cbTo,       SIGNAL(activated(int)), wgt, SLOT(setToLanguage(int)));
    connect(ui->translate,  SIGNAL(clicked(bool)),       SLOT(translateBtnClick()));
    connect(ui->tbFrom,     SIGNAL(textChanged()),       SLOT(textChange()));
    connect(ui->reverse,    SIGNAL(clicked(bool)),  wgt, SLOT(languageReverse()));

}





DefaultTranslator::~DefaultTranslator() {
    delete ui;
}





// Show/Hide options window
void DefaultTranslator::toggleOptionsShow(bool arg) {
    int duration = 500;
    optWgtPA->setDuration(duration);
    trnWgtPA->setDuration(duration);
    defWgtPA->setDuration(duration);

    if (arg) {                                                  // Show options
        disconnect(optWgtPA, SIGNAL(finished()), options, SLOT(hide()));
        disconnect(trnWgtPA, SIGNAL(finished()), transparentWgt, SLOT(hide()));
        ui->btnOptions->setChecked(false);

        transparentWgt->show();
        options->show();

        optWgtPA->setStartValue(QPoint(width(), 0));
        optWgtPA->setEndValue(QPoint(width() - options->width(), 0));

        trnWgtPA->setStartValue(0);
        trnWgtPA->setEndValue(0.4);

        defWgtPA->setStartValue(1);
        defWgtPA->setEndValue(10);

        optWgtPA->start();
        trnWgtPA->start();
        defWgtPA->start();

    } else if(options->isVisible()) {                           // Hide options
        connect(optWgtPA, SIGNAL(finished()), options,          SLOT(hide()));
        connect(trnWgtPA, SIGNAL(finished()), transparentWgt,   SLOT(hide()));

        if(optWgtPA->state() == QPropertyAnimation::Running) {  // Fine cancel first effect
            optWgtPA->stop();
            trnWgtPA->stop();
            defWgtPA->stop();

            QVariant v[3];                                      // After stop value changes still, maybe qt bug
            v[0] = optWgtPA->currentValue();
            v[1] = trnWgtPA->currentValue();
            v[2] = defWgtPA->currentValue();

            optWgtPA->setDuration(optWgtPA->currentTime());
            trnWgtPA->setDuration(trnWgtPA->currentTime());
            defWgtPA->setDuration(defWgtPA->currentTime());

            optWgtPA->setStartValue(v[0]);
            trnWgtPA->setStartValue(v[1]);
            defWgtPA->setStartValue(v[2]);
        } else {
            optWgtPA->setStartValue(QPoint(width() - options->width(), 0));
            trnWgtPA->setStartValue(0.4);
            defWgtPA->setStartValue(10);
        }

        optWgtPA->setEndValue(QPoint(width(), 0));
        trnWgtPA->setEndValue(0);
        defWgtPA->setEndValue(1);

        optWgtPA->start();
        trnWgtPA->start();
        defWgtPA->start();
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





// Necessary to change specific elements when the theme is changed
void DefaultTranslator::changeTheme(QString color) {
    QString prefix = color;
    if(color == "color")
        prefix = "black";
    ui->btnOptions->setIcon(QIcon(":/files/imgs/options-" + prefix + ".svg"));
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
