#include <QCoreApplication>
#include <QTextCodec>

#include "program.h"

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    Program prog(Q_NULLPTR);
    return a.exec();
}
