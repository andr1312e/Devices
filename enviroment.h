#ifndef ENVIROMENT_H
#define ENVIROMENT_H
#include "stdlib.h"

#include <QString>
#include <QProcess>

namespace Enviroment {
static QString GetEnviroment(const QString &name){
    const QString output=getenv(name.toLatin1().constData());
    return output;
}
static QString SetEnviroment(const QString &name, const QString &value)
{

    const QString command="sudo echo \"export " +name+"="+value+"\" >> /etc/environment";
    QProcess process;
    process.start(QLatin1Literal("/bin/sh"), QStringList()<< QLatin1Literal("-c") << command);
    process.waitForFinished(-1);
    return process.errorString();
}
}

#endif // ENVIROMENT_H
