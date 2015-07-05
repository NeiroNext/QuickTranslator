#ifndef SMARTTRANSLATE_H
#define SMARTTRANSLATE_H

#include <QObject>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QRegExp>
#include <QTextDocument>

class SmartTranslate : public QObject
{
    Q_OBJECT

public:
    SmartTranslate();
    ~SmartTranslate();

    void startTranslating();




private:
    void getNextPart(bool test);

    QClipboard   *cl;
    int  from, to;
    int packetCount;
    bool isFinish;

    QString buffer;
    QString result;


signals:
    void getPart(QString str);
    void finish();
    void getPacketCount(int);



public slots:
    void sendTranslateText(QString str);
    void afterFinish();


};

#endif // SMARTTRANSLATE_H
