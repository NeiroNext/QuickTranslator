#include <QDebug>
#include <QMessageBox>

#include "translate.h"
#include "widget.h"

using namespace std;

// Empty constructor
Translate::Translate(Widget *w){
    this->w = w;
    similarWords = true;
}





// Destructor
Translate::~Translate(){}





// Generate translate request
void Translate::setData(QString from, QString to, QByteArray text, GetTranslate *gt){
   setDataMain(from, to, text);

    this->gt = gt;

   connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(translateThis(QNetworkReply*)));
   connect(this,    SIGNAL(deleteManager()), manager, SLOT(deleteLater()));
}





// Main function for generate translate request
void Translate::setDataMain(QString from, QString to, QByteArray text) {
    QUrl url("http://translate.google.com/translate_a/single");

    QByteArray post;
    post.append("client=x");
    post.append("&sl=");         // From
    post.append(from);
    post.append("&tl=");         // To
    post.append(to);
    post.append("&dt=bd");       // Similar words
    post.append("&dt=t");        // Get translate
    post.append("&q=");          // Text

    lastTranslatedText = preTranslateProcessing(QString::fromUtf8(text));
    post.append(lastTranslatedText);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,
                  QVariant("application/x-www-form-urlencoded"));
    manager = new QNetworkAccessManager();
    manager->post(req, post);
}





// Return translate server answer
// And get data from the answer
void Translate::translateThis(QNetworkReply *rep){
   emit deleteManager();    // Delete manager memory

   if(!rep->error()){
      QByteArray ba = rep->readAll();

      // Replace SyntaxError path
      ba.replace("[,", "[[],");
      ba.replace(",,", ",[],");
      ba.replace(",,", ",[],");

      QVariantList vl = QxtJSON::parse(QString::fromUtf8(ba)).toList();
      /**
       *    0 - translate text
       *    1 - similar words
       *    2 - autodetect from language
       *    6 - autodetect percent
       *    8 - autodetect with percent
       */


      QVariantList vlist;
      QString autoLng;
      QString res = "";

      // Get auto detect language
      autoLng = vl[2].toString();


      // Get translate text lines
      vlist = vl[0].toList();

      // Get all text
      foreach (QVariant vline, vlist) {
         res += vline.toList()[0].toString();
      }

      // Get similar words and more more onother informations
      if(similarWords){
          vlist = vl[1].toList();
          if(vlist.size() > 0 && w->translateWindowType != Widget::TW_NOTIFIER){
              foreach (QVariant var, vlist) {
                 QVariantList subVl = var.toList();
                 QString str = subVl[0].toString();

                 res += "<br><br><b>" + tr("Similar words") + " (" + str + "): </b><br>";
                 foreach (QVariant var2, subVl[1].toList()) {
                     str = var2.toString();
                     res += str + "<br>";
                 }

              }
          }
      }

      if(gt == NULL)
          emit showTranslate(res, lastTranslatedText, autoLng);
      else
          gt->getTranslate(res, lastTranslatedText, autoLng);

   } else {
      qDebug() << "Server Error: " << rep->errorString();
      if(rep->error() == QNetworkReply::HostNotFoundError){
          QMessageBox::warning(0, tr("Host not found!"),
                                  tr("Network error! Host not found, maybe you offline now."));
      }
   }

}





// On/Off similar words of translation
void Translate::setSimilarWords(bool flag){
    this->similarWords = flag;
}





// Pre translate text processing
QString Translate::preTranslateProcessing(QString str) {
    str = QUrl::toPercentEncoding(str);

    str.replace("%D0%92%C2%A0", "%20"); // Space symbol for rich text


    return str;
}
