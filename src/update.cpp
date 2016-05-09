#include "update.h"
#include "widget.h"
#include <QDebug>





// Constructor
Update::Update(Widget *wgt) : QObject() {
    this->wgt = wgt;
    this->isCheked = false;

    url     = new QUrl("http://bimusoft.tk/projects/updates/");
    manager = new QNetworkAccessManager();

    connect(this,       SIGNAL(deleteManager()), manager,       SLOT(deleteLater()));
    connect(manager,    SIGNAL(finished(QNetworkReply*)),       SLOT(getResult(QNetworkReply*)));
}





// Destructor
Update::~Update() {
    delete url;

    if(!isCheked)
        delete manager;
}





// Check for updates
void Update::check() {
    isCheked = true;
    QByteArray post;
    post.append("name="+QApplication::applicationName());
    post.append("&version="+QApplication::applicationVersion());
    post.append("&id="+this->getMAC());

    QNetworkRequest req(*url);
    req.setHeader(QNetworkRequest::ContentTypeHeader,
                  QVariant("application/x-www-form-urlencoded"));
    manager->post(req, post);
}





// Get server answer
void Update::getResult(QNetworkReply *rep) {
    emit deleteManager();
    if(!rep->error()) {
        QByteArray answer = rep->readAll();
        if(answer.startsWith("json:")) {        // Has correct answer (site is alive)
            QVariantList list = QxtJSON::parse(answer.mid(5)).toList();

            bool    hasNew  = list[0].toBool();
            bool    errors  = list[1].toBool();
            QString ver     = list[2].toString();
            QUrl upUrl      = list[3].toUrl();
            QString info    = list[4].toString();

            if(!errors) {
                if(hasNew) {
                    int btn = QMessageBox::information(0, tr("Update available"),
                            tr("<center><h2>Update available</h2></center>"
                            "Available new version of the program!<br>"
                            "You are using a version: <b>%1</b> <br>"
                            "Available version: <b>%2</b> <br>"
                            "Do you want to download the new version?<br>%3")
                            .arg(QApplication::applicationVersion(), ver, info),
                            tr("Download new version"),
                            tr("Remind me later"));
                    if(btn == 0) {
                        QDesktopServices::openUrl(upUrl);
                    } else
                    if(btn == 1){
                        wgt->configUpdates(QVariant(),
                                           QDateTime::currentDateTime().addDays(3));
                    }
                } else {
                    wgt->configUpdates(QVariant(),
                                       QDateTime::currentDateTime().addDays(5));
                }
            } else {
                qDebug() << "Update error: " << info;
            }
        }
    }
}





// Return hashed computer MAC
// Only for counting the number of users
QByteArray Update::getMAC() {
    QByteArray ret = "";
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();

    if(list.size() > 0) {
        foreach (QNetworkInterface ni, list) {
           QString mac = ni.hardwareAddress();
           if(mac != "00:00:00:00:00:00" && mac != "" && ni.flags() & QNetworkInterface::IsUp) {
               ret += mac;
           }
        }
    }

    // Hide MAC address
    ret = QCryptographicHash::hash(ret, QCryptographicHash::Md5).toHex();

    return ret;
}
