#ifndef MESSAGECREATORS_STATEMESSAGEGETTER_H
#define MESSAGECREATORS_STATEMESSAGEGETTER_H

#include <QByteArray>

#include <QByteArray>
#include <QDataStream>
#include <QtEndian>
#include <QDebug>
#include <QtMath>
#include <QObject>
#include "constants.h"

#include "messageRepositories/ustrirovmessagerepository.h"

class UstirovMessageGetter:public QObject
{
    Q_OBJECT
public:
    explicit UstirovMessageGetter(const double f, const double fref,  UstrirovMessageRepository *messageRepository, QObject *parent);
    ~UstirovMessageGetter();
Q_SIGNALS:
    void ToAllDataCollected();
public:
    bool FillDataIntoStructFromMessage(const QByteArray &message);
    void SetNoConnectionState();
    void SetTimeOutState();
    const DevicesAdjustingKitMessage &GetMessage();
private:
    bool SaveFvcoToRepository(const QByteArray &message);
    bool SaveDoplerToRepository(const QByteArray &message);
    bool SaveDistanceToRepository(const QByteArray &message);
    bool SaveGainsToRepository(const QByteArray &message);
    bool SaveAttenuatorToRepository(const QByteArray &message);
    bool SaveWorkModeToRepository(const QByteArray &message);
private:
    quint16 GetIntFromMessage(const QByteArray &message) const;
    quint32 GetFractFromMessage(const QByteArray &message) const;
    bool GetDivFromMessage(const QByteArray &message) const;
    quint32 GetDistanceFromMessage(const QByteArray &message) const;
private:
    const quint8 m_indexInByteArrayOfGettingMessageId;
    const double f;
    const double c=299792458.0;
    const double Fref;
    const quint8 m_countOfWorkModes;
    UstrirovMessageRepository * const m_messageRepository;

};

#endif // MESSAGECREATORS_STATEMESSAGEGETTER_H
