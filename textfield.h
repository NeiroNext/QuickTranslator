#ifndef TEXTFIELD_H
#define TEXTFIELD_H

#include <QDialog>
#include <QTimer>
#include <QDesktopWidget>
#include "crossplatform.h"

namespace Ui {
    class TextField;
}


class TextField : public QDialog
{
    Q_OBJECT

public:
    explicit TextField(QWidget *parent = 0);
    ~TextField();

    void timerEvent(QTimerEvent *ev);

private:
    Ui::TextField *ui;

    QBasicTimer *timer;
    int         timerId;
    int         maxWidth;
    int         slideStep;

    QPoint      pos;


signals:
    void getText(QString str);


public slots:
    void show();
    void hide();
    void translateFromField();

};

#endif // TEXTFIELD_H
