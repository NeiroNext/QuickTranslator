#ifndef GRABLINEEDIT_H
#define GRABLINEEDIT_H

#include <QLineEdit>

class GrabLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit GrabLineEdit(QLineEdit *parent = 0);

    void setText(const QString str);

    void mousePressEvent(QMouseEvent *ev);


private:
    void keyPressEvent(QKeyEvent* ev);
    void keyReleaseEvent(QKeyEvent* ev);
    QString key;
    QString lastHotkey;
    bool    infoShow;



signals:
    void hotkeyChanged(GrabLineEdit *gle, QString key);

public slots:

};

#endif // GRABLINEEDIT_H
