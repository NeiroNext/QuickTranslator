#ifndef GRABLINEEDIT_H
#define GRABLINEEDIT_H

#include <QLineEdit>

class GrabLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit GrabLineEdit(QWidget *parent = 0);



private:
    void keyPressEvent(QKeyEvent* ev);
    void keyReleaseEvent(QKeyEvent* ev);
    QString key;

signals:
    void hotkeyChanged(QString key);

public slots:

};

#endif // GRABLINEEDIT_H
