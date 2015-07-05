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
void Translate::setData(QString from, QString to, QByteArray text){
   QUrl url("http://translate.google.com/translate_a/t");
   
   QByteArray post;
   post.append("client=x");
   post.append("&sl=");         // From
   post.append(from);
   post.append("&tl=");         // To
   post.append(to);
   post.append("&dt=bd");       // Similar words
   post.append("&text=");       // Text


   text = QUrl::toPercentEncoding(QString::fromUtf8(text));
   post.append(text);

   QNetworkRequest req(url);
   req.setHeader(QNetworkRequest::ContentTypeHeader,
                 QVariant("application/x-www-form-urlencoded"));
   manager = new QNetworkAccessManager();
   manager->post(req, post);

   connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(translateThis(QNetworkReply*)));
   connect(this,    SIGNAL(deleteManager()), manager, SLOT(deleteLater()));
 }





// Return translate server answer
// And get data from the answer
void Translate::translateThis(QNetworkReply *rep){
   emit deleteManager();    // Delete manager memory

   if(!rep->error()){
      QByteArray ba = rep->readAll();

      QVariantMap varmap = QxtJSON::parse(QString::fromUtf8(ba)).toMap();
      QVariantList vlist;
      QString res = "";

      // Get translate text
      vlist = varmap["sentences"].toList();

      foreach (QVariant var, vlist) {
         QVariantMap varmap2 = var.value<QVariantMap>();
         QString str = varmap2["trans"].toString();
         res += str;
      }

      // Get similar words and more more onother informations
      if(similarWords){
          vlist = varmap["dict"].toList();
          if(vlist.size() > 0 && w->translateWindowType != Widget::TW_NOTIFIER){
              foreach (QVariant var, vlist) {
                 QVariantMap varmap2 = var.value<QVariantMap>();
                 QVariantList vlist2 = varmap2["entry"].toList();
                 QString str;

                 str = varmap2["pos"].toString();
                 res += "<br><br><b>" + tr("Similar words") + " (" + str + "): </b><br>";
                 foreach (QVariant var2, vlist2) {
                     QVariantMap varmap3 = var2.value<QVariantMap>();
                     str = varmap3["word"].toString();
                     res += str + "<br>";
                 }

              }
          }
      }


      emit showTranslate(res);

   } else {
      qDebug() << "Server Error: " << rep->errorString();
      if(rep->error() == QNetworkReply::HostNotFoundError){
          QMessageBox::warning(0, tr("Host not found!"), "<font color="+w->getTTColor()+">"+
                                  tr("Network error! Host not found, maybe you offline now.")+
                                  "</font>");
      }
   }


}





// On/Off similar words of translation
void Translate::setSimilarWords(bool flag){
    this->similarWords = flag;
}
