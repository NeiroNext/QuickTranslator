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
    QListWidget *fromLngList,
                *toLngList;
    QPair<QList<QListWidgetItem*>, QList<QListWidgetItem*> > listWgtItms;
    QAction     *copyAll;
    QAction     *copySel;

    bool         fly;
    QRect        defaultRect;
    QPoint       mousePos;



public slots:
    void showTranslate(QString str);
    void copyToBuffer (QAction *act);
    void show();
    void hide();
    void showFromMenu();
    void showToMenu();
    void setFromIndex(QModelIndex index);
    void setToIndex(QModelIndex index);
};

#endif // BOX_H
