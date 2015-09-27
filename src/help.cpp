#include "help.h"
#include "ui_help.h"
#include "crossplatform.h"

Help::Help(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Help)
{
    ui->setupUi(this);

}

Help::~Help()
{
    delete ui;
}





// Initialization help source
void Help::Init(QString lng) {
    QString uri = QApplication::applicationDirPath() + "/help/" + lng;
    uri         = Crossplatform::_FilePathPrefix() + uri;
    ui->textBrowser->setSearchPaths(QStringList() << uri);
    ui->textBrowser->setSource(QUrl(uri + "/index.html"));
}
