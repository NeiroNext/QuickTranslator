#ifndef BOX_H
#define BOX_H

#include <QDialog>
#include <QMenu>
#include <QTimer>
#include <QRect>
#include <QMouseEvent>
#include <QClipboard>
#include <QDesktopWidget>
#include <QListWidgetItem>
#include <QComboBox>
#include "crossplatform.h"

namespace Ui {
    class Box;
}
class Widget;

class Box : public QDialog
{
    Q_OBJECT

public:
    explicit Box(Widget *wgt, QWidget *parent = 0);
    ~Box();

    void setFly(bool isFly);
    void loadLanguages(QList<QListWidgetItem*> itemsFrom);
    void loadLanguages(QStringList items);

    QComboBox *fromLng, *toLng;



private:
    void mousePressEvent(QMouseEvent *ev);
    void timerEvent(QTimerEvent *ev);
    bool event(QEvent *ev);

    Widget *wgt;

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
