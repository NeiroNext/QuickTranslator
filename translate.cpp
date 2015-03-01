#include <QDebug>
#include <QMessageBox>

#include "translate.h"
#include "widget.h"

using namespace std;

// Empty constructor
Translate::Translate(Widget *w){
    this->w = w;
}





// Destructor
Translate::~Translate(){}





// Generate translate request
void Translate::setData(QString from, QString to, QByteArray text){
   QUrl url("http://translate.google.ru/translate_a/t");
   
   QByteArray post;
   post.append("client=x");
   post.append("&sl=");
   post.append(from);
   post.append("&tl=");
   post.append(to);
   post.append("&text=");

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
      QVariantList vlist = varmap["sentences"].toList();
      QString res = "";

      foreach (QVariant var, vlist) {
         QVariantMap varmap2 = var.value<QVariantMap>();
         QString str = varmap2["trans"].toString(); 
         res += str;
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

