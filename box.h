#ifndef BOX_H
#define BOX_H

#include <QDialog>
#include <QMenu>
#include <QTimer>
#include <QRect>

namespace Ui {
    class Box;
}

class Box : public QDialog
{
    Q_OBJECT

public:
    explicit Box(QWidget *parent = 0);
    ~Box();

    void setFly(bool isFly);




private:
    void mousePressEvent(QMouseEvent *ev);
    void timerEvent(QTimerEvent *ev);
    bool event(QEvent *ev);


    QBasicTimer *timerShow,
                *timerHide;
    int          timerIdShow,
                 timerIdHide;
    int          defY,
                 nowY;

    Ui::Box     *ui;
    QMenu       *menu;
    QAction     *copyAll;
    QAction     *copySel;

    bool         fly;
    QRect        defaultRect;



public slots:
    void showTranslate(QString str);
    void copyToBuffer (QAction *act);
    void show();
    void hide();
};

#endif // BOX_H
