#include <QApplication>
#include <QClipboard>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextStream ts(stdout);
    QClipboard *cb = app.clipboard();
    ts << cb->text(QClipboard::Selection);

    return 0;
}
