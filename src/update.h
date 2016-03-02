#ifndef UPDATE_H
#define UPDATE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QApplication>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDateTime>
#include <QCryptographicHash>
#include "qxtjson.h"

class Widget;

class Update : public QObject
{
    Q_OBJECT
public:
    explicit Update(Widget *wgt);
    ~Update();
    void check();

    const QUrl *url;

private:
    QByteArray getMAC();
    QNetworkAccessManager *manager;

    Widget *wgt;



signals:
    void deleteManager();


public slots:
    void getResult(QNetworkReply *rep);

};

#endif // UPDATE_H
