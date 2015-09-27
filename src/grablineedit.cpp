#include <QKeyEvent>
#include "grablineedit.h"
#include "crossplatform.h"
#include <QMenu>
#include <QDebug>



// Empty Constructor
GrabLineEdit::GrabLineEdit(QLineEdit *parent) :
    QLineEdit(parent)
{
    this->setMinimumSize( parent->minimumSize() );
    this->setMaximumSize( parent->maximumSize() );
    this->setGeometry   ( parent->geometry()    );
    this->setSizePolicy ( parent->sizePolicy()  );
    this->setText       ( parent->text()        );

    this->setReadOnly(true);
    this->setContextMenuPolicy(Qt::CustomContextMenu); // Disable standart menu
    infoShow = false;
}





// Grab pressed keys in the QLineEdit
void GrabLineEdit::keyPressEvent(QKeyEvent *ev){
    int k   = ev->key();
    int mod = ev->modifiers();
    key     = "";

    if(mod & Qt::CTRL)      key += "Ctrl+";
    if(mod & Qt::SHIFT)     key += "Shift+";
    if(mod & Qt::ALT)       key += "Alt+";
    if(mod & Qt::META)      key += "Meta+";

    switch(k){
    case Qt::Key_Tab:
        key += "Tab";
        break;

    case Qt::Key_Left:
        key += "Left";
        break;

    case Qt::Key_Up:
        key += "UP";
        break;

    case Qt::Key_Right:
        key += "Right";
        break;

    case Qt::Key_Down:
        key += "Down";
        break;

    case Qt::Key_Pause:
        key += "Pause";
        break;

    case Qt::Key_Delete:
        key += "Del";
        break;

    case Qt::Key_Backspace:
        key += "Backspace";
        break;

    case Qt::Key_Home:
        key += "Home";
        break;

    case Qt::Key_End:
        key += "End";
        break;

    case Qt::Key_Enter:
        key += "Enter";
        break;

    case Qt::Key_Return:
        key += "Return";
        break;

    default:
        if(k >= Qt::Key_F1 && k <= Qt::Key_F35)
            key += QString("F") + QString::number(k - Qt::Key_F1 + 1);
        else if(k == Qt::Key_Escape)
            key = lastHotkey;
        else if(k < 16777248)
            key += QString((QChar)k).toUpper();


    }


    key = key.replace("Meta", Crossplatform::_MetaKeyName());
    this->setText(key);
}





// Send signal when key released
void GrabLineEdit::keyReleaseEvent(QKeyEvent *ev){
    key = key.replace(Crossplatform::_MetaKeyName(), "Meta");
    emit hotkeyChanged(this, key);
}





// SetText function
void GrabLineEdit::setText(const QString str) {
    QString str2 = str;

    if(!infoShow)
        lastHotkey = str;

    str2 = str2.replace("Meta", Crossplatform::_MetaKeyName());
    QLineEdit::setText(str2);
    infoShow = false;
}





// Show info message when mouse pressa
void GrabLineEdit::mousePressEvent(QMouseEvent *ev) {
    infoShow = true;
    this->setText(tr("Press key combination"));
}

