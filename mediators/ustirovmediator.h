#ifndef MEDIATORS_USTIROVMEDIATOR_H
#define MEDIATORS_USTIROVMEDIATOR_H

#include <QObject>
#include <queue>
#include <QSettings>
#include <QTextCodec>

#include "messageCreatrors/statemessagesender.h"
#include "messageCreatrors/statemessagegetter.h"

#include "sockets/ustirovsocket.h"

class UstirovMediator : public QObject
{
    Q_OBJECT
public:
    explicit UstirovMediator(const QString &settingsFileName, QObject *parent);
    ~UstirovMediator();
private:
    void ReadDataFromSettingsFile(const QString &settingsFileName);
    void CreateObjects();
    void ConnectObjects();
Q_SIGNALS:
    void ToSendRarmUstirovState(DevicesAdjustingKitMessage &upcbState);
public Q_SLOTS:
    void OnSetUstirovState(const DevicesAdjustingKitMessage &state);
    void OnGetUstirovState();
private Q_SLOTS:
    void OnGetMessageWithState(const QByteArray &message);
    void OnNoAnswerGet();
    void OnSendingNext();
    void OnResetQueue();
    void OnAllDataCollected();
private:
    void SetStateCommandsCreate(const DevicesAdjustingKitMessage &state);
    void GetStateCommandsCreate();
private:
    QString m_ustirovAdress;
    quint16 m_ustirovPort;
    UstirovSocket *m_ustirovSocket;
    StateMessageSender *m_setMessageCreator;
    StateMessageGetter *m_stateMessageGetter;
    std::queue<QByteArray> *m_messagesToSendQueue;
};

#endif // MEDIATORS_USTIROVMEDIATOR_H
