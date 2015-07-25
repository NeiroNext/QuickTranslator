#include "textfield.h"
#include "ui_textfield.h"
#include <QDebug>





// Constructor
TextField::TextField(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TextField)
{
    ui->setupUi(this);
    setWindowFlags(Crossplatform::_WindowOnTopFrameIconHide());
    setMaximumWidth(0);
    maxWidth   = 400;
    slideStep  = 30;

    timer = new QBasicTimer();


    connect(ui->pushButton, SIGNAL(clicked()), SLOT(translateFromField()));
}





// Destructor
TextField::~TextField()
{
    delete ui;
    delete timer;
}





// Show slot
void TextField::show(){
    if(!timer->isActive()){
        if(!isVisible()){
            slideStep  = 30;
            timer->start(30, this);
            timerId = timer->timerId();
            ui->lineEdit->setFocus(Qt::ActiveWindowFocusReason);
            QWidget::show();

            Crossplatform::setFocus(this);

            QRect rect = QApplication::desktop()->screenGeometry();
            pos = QPoint(rect.width()/2, rect.height()/2 - this->height()/2);
            this->move(pos);
        }
    } else {
        pos.setX(pos.x() - slideStep/2);
        setMinimumWidth(width() + slideStep);
        setMaximumWidth(width() + slideStep);
        move(pos.x(), pos.y());
    }
}





// Hide slot
void TextField::hide(){
    if(!timer->isActive()){
        slideStep  = -30;
        timer->start(30, this);
        timerId = timer->timerId();
    } else {
        pos.setX(pos.x() - slideStep/2);
        setMinimumWidth(width() + slideStep);
        setMaximumWidth(width() + slideStep);
        move(pos.x(), pos.y());
    }
}





// Timer Events
void TextField::timerEvent(QTimerEvent *ev){
    if(ev->timerId() == timerId){
        if(slideStep > 0) {
            show();
            if(this->width() >= maxWidth) {
                timer->stop();
            }
        } else {
            hide();
            if(this->width() <= -slideStep) { // Now slideStep negative
                timer->stop();
                QWidget::hide();
            }
        }
    }
}





// Translate text frome text field
void TextField::translateFromField(){
    hide();
    emit getText(ui->lineEdit->text());
}
