#include "smarttranslate.h"
#include <QDebug>
#include <QTextCodec>
#include "crossplatform.h"


// Constructor
SmartTranslate::SmartTranslate() : QObject() {
    connect(this, SIGNAL(finish()), SLOT(afterFinish()));
}





// Destructor
SmartTranslate::~SmartTranslate(){ }





// Start of smart translating
void SmartTranslate::startTranslating(){
    to          = 0;
    packetCount = 0;
    result      = "";
    isFinish    = false;

    buffer      = Crossplatform::clipboard();

    if(buffer == ""){
        emit finish();
        return;
    } else if(!buffer.contains("<")){
        emit getPart(buffer);
        isFinish = true;
        return;
    }


    // Twice parsing, count knowing part's count and normal
    for(int i=0; i<2; i++) {
        // If full HTML, for go trougth header scripts and styles
        if(buffer.contains("<!DOCTYPE HTML", Qt::CaseInsensitive) && buffer.indexOf("<body", 0, Qt::CaseInsensitive)) {
            to = buffer.indexOf("<body", 0, Qt::CaseInsensitive);
            result += buffer.mid(0, to);
        }

        getNextPart(i==0);  // Testing parse for knowing part's count / normal parsing

        if(i == 0) {
            to          = 0;
            result      = "";
            isFinish    = false;
        }
    }

}





// Get next part of xml/html document
void SmartTranslate::getNextPart(bool test){
    int tmpFrom;

    from = buffer.indexOf('<', to);
    tmpFrom = from;

    while (true){
       to   = buffer.indexOf('>', from);

       if (to + 3 <= buffer.length()){
           if (buffer.at(to+1) == '<' ||    // ...><a href...
               buffer.at(to+2) == '<'){    // ...> <a href... | ...>=<a href...
               from = to + 2;
           } else break;
       } else break;
    }
    from = tmpFrom;

    result += buffer.mid(from, to-from+1);

    from = to + 1;
    to   = buffer.indexOf('<', from);

    if (to == -1){
        if(!test) {
            emit finish();
        } else {
            emit getPacketCount(packetCount);
        }
        return;
    }

    QString tmpStr = buffer.mid(from, to-from);

    if(tmpStr.contains(QRegExp("^([\\W\\d\\_]|&[a-zA-Z]+;)+$"))){
        result += tmpStr;
        getNextPart(test);
    } else {
        if(!test)
            emit getPart(tmpStr);
        else {
            packetCount ++;
            getNextPart(test);
        }
    }
}





// Slot for recive next translated text part
// And it some processing
void SmartTranslate::sendTranslateText(QString translate, QString origin){
    // Some processing
    if(!translate.startsWith(" ") && origin.startsWith("%20")) {
        while(origin.startsWith("%20") && origin.length() >= 3) {
            translate = " " + translate;
            origin = origin.mid(3);
        }
    }
    if(!translate.endsWith(" ") && origin.endsWith("%20")) {
        while(origin.endsWith("%20") && origin.length() >= 3) {
            translate = translate + " ";
            origin = origin.mid(0, origin.length()-3);
        }
    }



    result += Qt::escape(translate);

    if(!isFinish) {
        getNextPart(false);
    } else {
        emit finish();
    }
}





// After finish processes
void SmartTranslate::afterFinish(){
    QMimeData *md = new QMimeData();
    md->setData("text/html", result.toUtf8()); // text/html data
    QTextDocument td;
    td.setHtml(result);
    md->setText(td.toPlainText());
    QApplication::clipboard()->setMimeData(md);// text data
}

