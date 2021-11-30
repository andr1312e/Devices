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

class StateMessageGetter:public QObject
{
    Q_OBJECT
public:
    StateMessageGetter(QObject *parent);
    ~StateMessageGetter();
    bool FillDataIntoStructFromMessage(const QByteArray &message);
    void SetBadState();
    const DevicesAdjustingKitMessage &GetMessage() const;
Q_SIGNALS:
    void ToAllDataCollected();
private:
    bool GetFvcoFromFirstMessage(const QByteArray &message);
    bool GetDoplerFromSecondMessage(const QByteArray &message);
    bool GetDistanceFromThirdMessage(const QByteArray &message);
    bool GetGainTxGainRXFromFourthMessage(const QByteArray &message);
    bool GetAttenuatorRXFromFiveMessage(const QByteArray &message);
    bool GetWorkModeFromSixMessage(const QByteArray &message);
private:
    const quint8 m_indexInByteArrayOfGettingMessageId;
    const quint8 m_maxWorkModeIndex;
    UstrirovMessageRepository *m_repository;

};

#endif // MESSAGECREATORS_STATEMESSAGEGETTER_H
