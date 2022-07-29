#ifndef ENVIROMENT_H
#define ENVIROMENT_H

#include <QDebug>
#include <QString>
#include <QProcess>

namespace ProFile
{
static inline QString GetProFileField(const QString &name)
{
    const QString output = QProcessEnvironment::systemEnvironment().value(name);
    return output;
}

static inline QString SetProfileField(const QString &name, const QString &value)
{
    const QString text = QLatin1String("export ") + name + QLatin1String("=") + value;
    const QString filePath = QLatin1String("/etc/bash.bashrc");
    const QString command = QStringLiteral("sudo sh -c \"echo -e \"'") + text + QStringLiteral("'\" >> '") + filePath + QStringLiteral("'\"");
    QProcess process;
    process.start(QLatin1String("/bin/sh"), QStringList() << QLatin1String("-c") << command);
    process.waitForFinished(-1);
    return process.errorString();
}

static inline void ApplyTextToProfile()
{
    const QString command = QLatin1String("source /etc/profile");
    QProcess process;
    process.start(QLatin1String("/bin/sh"), QStringList() << QLatin1String("-c") << command);
    process.waitForFinished(-1);
}
}

#endif // ENVIROMENT_H
