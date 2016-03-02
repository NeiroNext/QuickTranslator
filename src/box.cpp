#include "box.h"
#include "ui_box.h"
#include "widget.h"

#include <QDebug>



// Constructor
Box::Box(Widget *wgt, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Box)
{
    ui->setupUi(this);
    setWindowFlags(Crossplatform::_WindowOnTopFrameIconHide());

    this->wgt   = wgt;

    fly         = false;

    timerShow   = new QBasicTimer();
    timerHide   = new QBasicTimer();

    menu        = new QMenu(this);
    fromLngList = new QListWidget(this);
    toLngList   = new QListWidget(this);
    fromLngList->setWindowFlags(Crossplatform::_WindowOnTopFrameIconHide());
    toLngList->setWindowFlags(Crossplatform::_WindowOnTopFrameIconHide());
    fromLngList->setStyleSheet("QListWidget {border: 1px solid lightgray;}");
    toLngList->setStyleSheet("QListWidget {border: 1px solid lightgray;}");
    copyAll = new QAction(tr("Copy All"), 0);
    copySel = new QAction(tr("Copy Selected"), 0);

    menu->addAction(copyAll);
    menu->addAction(copySel);

    connect(menu,       SIGNAL(triggered(QAction*)),        SLOT(copyToBuffer(QAction*)));
    connect(ui->close,  SIGNAL(clicked()),                  SLOT(hide()));
    connect(ui->reverse,SIGNAL(clicked()), wgt,             SLOT(languageReverse()));
    connect(ui->from,   SIGNAL(clicked(bool)),              SLOT(showFromMenu()));
    connect(ui->to,     SIGNAL(clicked(bool)),              SLOT(showToMenu()));
    connect(fromLngList,SIGNAL(clicked(QModelIndex)), wgt,  SLOT(setFromLanguage(QModelIndex)));
    connect(toLngList,  SIGNAL(clicked(QModelIndex)), wgt,  SLOT(setToLanguage(QModelIndex)));


    defaultRect = rect();
}





// Destructor
Box::~Box(){
    delete ui;
    delete copyAll;
    delete copySel;
    delete menu;
    delete timerShow;
    delete timerHide;
    delete fromLngList;
    delete toLngList;
}





// Show translated text
void Box::showTranslate(QString str){
    ui->textEdit->setText(str);
    QRect rect = QApplication::desktop()->screenGeometry();
    if(!fly){
        QRect rect2 =QApplication::desktop()->availableGeometry();

        int heightDif = rect.height() - rect2.height();         // bottom panel size
        int yDif =  rect2.y() - rect.y();                       // top    panel size

        rect.setX(rect.width()  - width());
        rect.setY(rect.height() - height() - heightDif + yDif);
        rect.setWidth(width());
        rect.setHeight(height());

        if(!timerShow->isActive() && !timerHide->isActive() && this->isHidden()){
            defY = rect.y();
            nowY = rect.y() + height() + 60;
        }

        move(rect.x(), rect.y());
    } else {
        QPoint posBR    = this->pos() + QPoint(width(), height());  // Bottom-Right position
        QPoint newPoint = this->pos();
        if(posBR.x() >= rect.width())
            newPoint.setX(rect.width() - width() - 1);
        if(posBR.y() >= rect.height())
            newPoint.setY(rect.height() - height() - 1);
        move(newPoint);
    }
    show();
}





// Copy translated text into clipboard
void Box::copyToBuffer(QAction *act){
   if(act == copyAll)
        QApplication::clipboard()->setText(ui->textEdit->toPlainText());
   else {
        ui->textEdit->copy();
   }
}





// Set fly state
void Box::setFly(bool isFly){
    if(isFly != fly){
        fly = isFly;
        if(isFly) {
            ui->close->hide();
            setGeometry(0, 0, 240, 140);
        } else {
            ui->close->show();
            setGeometry(defaultRect);
        }
    }
}





// Show menu by mouse right key pressdown
void Box::mousePressEvent(QMouseEvent *ev){
    if(ev->button() & Qt::RightButton && ev->pos().y() > 20){
        menu->exec(ev->globalPos());
    }
}





// Show slot
void Box::show(){
    if(!timerShow->isActive() && !timerHide->isActive() && this->isHidden()){
        if(!fly){
            move(x(), nowY);
            timerShow->start(10, this);
            timerIdShow = timerShow->timerId();
            QWidget::show();
        } else {
            QWidget::show();
            activateWindow();
            Crossplatform::setFocus(this);
        }
    }
}





// Hide slot
void Box::hide(){
    fromLngList->hide();
    toLngList->hide();

    if(!timerHide->isActive() && !timerShow->isActive()){
        if(!fly){
            timerHide->start(10, this);
            timerIdHide = timerHide->timerId();
        } else {
            QWidget::hide();
        }
    }
}





// Timer Events
void Box::timerEvent(QTimerEvent *ev){
    if(ev->timerId() == timerIdShow){
        nowY -= 9;

        if(nowY <= defY){
            timerShow->stop();
            nowY = defY;
        }
    } else if (ev->timerId() == timerIdHide) {
        nowY += 9;

        if(nowY >= defY+height()*2+60){
            timerHide->stop();
            QWidget::hide();
        }
    }

    move(x(), nowY);
}





// Events
bool Box::event(QEvent *ev){
    if(ev->type() == QEvent::WindowDeactivate && fly)
        hide();
    return QWidget::event(ev);
}





// Load language list with icon
void Box::loadLanguages(QList<QListWidgetItem*> items) {
    listWgtItms.first.clear();
    listWgtItms.second.clear();

    for(int i=0; i<items.size(); i++) {
        QListWidgetItem *item1 = new QListWidgetItem(*items[i]);
        listWgtItms.first.append(item1);

        fromLngList->addItem(listWgtItms.first[i]);
        if(i > 0) {
            QListWidgetItem *item2 = new QListWidgetItem(*items[i]);
            listWgtItms.second.append(item2);
            toLngList->addItem(listWgtItms.second[i-1]);
        }
    }
}

// Load language list without icon
void Box::loadLanguages(QStringList items) {
    for(int i=0; i<items.size(); i++) {
        fromLngList->addItem(items[i]);
        if(i > 0) {
            toLngList->addItem(items[i]);
        }
    }
}





// Show from language menu slot
void Box::showFromMenu() {
    toLngList->hide();
    if(fromLngList->isVisible()) {
        fromLngList->hide();
    } else {
        QPoint pos = this->pos() + ui->from->pos() + QPoint(0, ui->from->height());
        fromLngList->setGeometry(QRect(pos.x(),
                                       pos.y(),
                                       200,
                                       this->height()-ui->to->height()));
        fromLngList->show();
        fromLngList->scrollToItem(fromLngList->currentItem());
        Crossplatform::setFocus(fromLngList);
    }
}





// Show to language menu slot
void Box::showToMenu() {
    fromLngList->hide();
    if(toLngList->isVisible()) {
        toLngList->hide();
    } else {
        QPoint pos = this->pos() + ui->to->pos() + QPoint(0, ui->to->height());
        toLngList->setGeometry(QRect(pos.x(),
                                     pos.y(),
                                     200,
                                     this->height()-ui->to->height()));
        toLngList->show();
        toLngList->scrollToItem(toLngList->currentItem());
        Crossplatform::setFocus(toLngList);
    }
}





// Change from language menu index
void Box::setFromIndex(QModelIndex index) {
    int indx = index.row();
    fromLngList->setCurrentRow(indx);
    ui->from->setIcon(fromLngList->item(indx)->icon());
}





// Change to language menu index
void Box::setToIndex(QModelIndex index) {
    int indx = index.row();
    toLngList->setCurrentRow(indx);
    ui->to->setIcon(toLngList->item(indx)->icon());
}
