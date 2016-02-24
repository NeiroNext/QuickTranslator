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

    this->wgt     = wgt;
    this->fromLng = ui->cbFrom;
    this->toLng   = ui->cbTo;

    fly = false;

    timerShow   = new QBasicTimer();
    timerHide   = new QBasicTimer();

    menu    = new QMenu(this);
    copyAll = new QAction(tr("Copy All"), 0);
    copySel = new QAction(tr("Copy Selected"), 0);

    menu->addAction(copyAll);
    menu->addAction(copySel);

    connect(menu,      SIGNAL(triggered(QAction*)), SLOT(copyToBuffer(QAction*)));
    connect(ui->close, SIGNAL(clicked()),           SLOT(hide()));
    connect(ui->cbFrom,SIGNAL(activated(int)), wgt, SLOT(setFromLanguage(int)));
    connect(ui->cbTo,  SIGNAL(activated(int)), wgt, SLOT(setToLanguage(int)));

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
    if(ev->button() & Qt::RightButton){
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
    QString spaces = "          ";    // Cratch for non cut text at right
    for(int i=0; i<items.size(); i++) {
        ui->cbFrom->addItem(items[i]->icon(), items[i]->text()+spaces);
        ui->cbTo->addItem(items[i]->icon(), items[i]->text()+spaces);
    }
    ui->cbTo->removeItem(0);
}

// Load language list without icon
void Box::loadLanguages(QStringList items) {
    QString spaces = "                    ";    // Cratch for non cut text at right
    for (int i=0; i<items.size(); i++) {
        items[i] += spaces;
    }
    ui->cbFrom->addItems(items);
    ui->cbTo->addItems(items);
    ui->cbTo->removeItem(0);
}
