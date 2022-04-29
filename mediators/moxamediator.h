#ifndef MOXAMEDIATOR_H
#define MOXAMEDIATOR_H

#include <QObject>
#include <QSettings>
#include <QProcess>

#include "datamessage.h"
#include "mediators/logger.h"

class MoxaMediator : public QObject
{
    Q_OBJECT
public:
    explicit MoxaMediator(const QString &settingsFileName, QObject *parent);
    ~MoxaMediator();
private:
    void ReadDataFromSettingsFile(const QString &settingsFileName);
    void InitObjects();
Q_SIGNALS:
    void ToSendRarmMoxaWorksState(DevicesMoxaStateMessage &moxaState);
public:
    const QString &GetMoxaIpAdress() const;
    bool IsMoxaConnected() const;
protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    QStringList m_processParametrs;
    QString m_moxaIpAdress;
    quint16 m_moxaCheckMiliseconds;
    QProcess m_process;
    DevicesMoxaStateMessage moxaState;
};

#endif // MOXAMEDIATOR_H
