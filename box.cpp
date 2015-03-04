#include <QMouseEvent>
#include <QClipboard>
#include <QDesktopWidget>
#include "box.h"
#include "ui_box.h"

#include <QDebug>



// Constructor
Box::Box(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Box)
{
    ui->setupUi(this);

#ifdef Q_OS_LINUX
    // Qt::X11BypassWindowManagerHint for hide dialog (Box class) icon in the KDE panel
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
#endif
#ifdef Q_OS_WIN
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
#endif
    timerShow   = new QBasicTimer();
    timerHide   = new QBasicTimer();

    menu    = new QMenu(this);
    copySel = new QAction(tr("Copy Selected"), 0);
    copyAll = new QAction(tr("Copy All"), 0);

    menu->addAction(copySel);
    menu->addAction(copyAll);

    connect(menu,      SIGNAL(triggered(QAction*)), SLOT(copyToBuffer(QAction*)));
    connect(ui->close, SIGNAL(clicked()),           SLOT(hide()));


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

    setGeometry(rect);
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





// Show menu by mouse right key pressdown
void Box::mousePressEvent(QMouseEvent *ev){
    if(ev->button() & Qt::RightButton){
        menu->exec(ev->globalPos());
    }
}





// Show slot
void Box::show(){
    if(!timerShow->isActive() && !timerHide->isActive() && this->isHidden()){
        move(x(), nowY);
        QWidget::show();
        timerShow->start(10, this);
        timerIdShow = timerShow->timerId();
    }
}





// Hide slot
void Box::hide(){
    if(!timerHide->isActive() && !timerShow->isActive()){
        timerHide->start(10, this);
        timerIdHide = timerHide->timerId();
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


