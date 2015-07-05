#ifndef GRABLINEEDIT_H
#define GRABLINEEDIT_H

#include <QLineEdit>

class GrabLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit GrabLineEdit(QLineEdit *parent = 0);



private:
    void keyPressEvent(QKeyEvent* ev);
    void keyReleaseEvent(QKeyEvent* ev);
    QString key;

signals:
    void hotkeyChanged(GrabLineEdit *gle, QString key);

public slots:

};

#endif // GRABLINEEDIT_H
