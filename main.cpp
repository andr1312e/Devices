#include <QCoreApplication>

#include "devices.h"
#include "singleinstancemaker.h"

int main(int argc, char *argv[])
{
    SingleInstanceMaker guard(QStringLiteral("DEVICES"));
    if (guard.CanRunApp())
    {
        QCoreApplication applicaton(argc, argv);
        Devices devices;
        return applicaton.exec();
    }
    else
    {
        qDebug() << "Невозможно выделить память. Проверьте системый монитор на наличие процесса devices";
        return -1;
    }
}
