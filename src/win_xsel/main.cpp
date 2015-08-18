#include <QApplication>
#include <QClipboard>
#include <windows.h>
#include <QTextStream>
#include <QTextCodec>
#include <QMimeData>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextStream ts(stdout);

    Sleep(500);
    // Simulate a key press
    keybd_event(  VK_CONTROL,
                  0x45,
                  KEYEVENTF_EXTENDEDKEY | 0,
                  0 );
    keybd_event(  (BYTE)'C',
                  0x45,
                  KEYEVENTF_EXTENDEDKEY | 0,
                  0 );


    // Simulate a key release
    keybd_event( (BYTE)'C',
                  0x45,
                  KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                  0);
    keybd_event(  VK_CONTROL,
                  0x45,
                  KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,
                  0);

    // Windows like the hard programming ))))
    QString ret;
    QTextCodec *tc = QTextCodec::codecForName("Windows-1251");
    QByteArray ba;
    ba.append(QApplication::clipboard()->mimeData()->data("text/plain"));
    ret = tc->toUnicode(ba);
    ts << ret;

    return 0;
}
