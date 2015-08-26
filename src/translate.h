#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextEdit>
#include <QUrl>
#include <qxtjson.h>

class Widget;

class Translate : public QObject
{
   Q_OBJECT
   
public:
   Translate(Widget *w);
   ~Translate();
   void setData(QString from, QString to, QByteArray text);

   void setSimilarWords(bool flag);


private:
      QString preTranslateProcessing(QString str);

      QNetworkAccessManager *manager;
      Widget *w;

      bool     similarWords;
      QString  lastTranslatedText;



private slots:
   void translateThis(QNetworkReply* rep);

signals:
   void showTranslate(QString translate, QString origin);
   void deleteManager();


};

#endif // TRANSLATE_H
