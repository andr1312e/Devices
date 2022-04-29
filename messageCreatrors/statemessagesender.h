#ifndef MESSAGECREATORS_STATEMESSAGESENDER_H
#define MESSAGECREATORS_STATEMESSAGESENDER_H

#include <QByteArray>
#include <QIODevice>
#include <QDataStream>
#include <QVarLengthArray>
#include <QSharedPointer>
#include <cmath>

#include "constants.h"

#include "messageRepositories/ustrirovmessagerepository.h"

class UstirovMessageSender
{
public:
    explicit UstirovMessageSender(const double f, const double fref);
    ~UstirovMessageSender();
    const QByteArray CreateZeroCommand() const;
    const QByteArray CreateFirstCommand(double fvcoFreq) const;
    const QByteArray CreateSecondCommand(double fvcoFreq, double doplerFreq) const;
    const QByteArray CreateThirdCommand(double distance, double distanceToLocator) const;
    const QByteArray CreateFourthCommand(double gainTX, double gainRX) const;
    const QByteArray CreateFiveCommand(double attenuator) const;
    const QByteArray CreateSixCommand(double workMode) const;
    const QByteArray CreateSevenCommand(quint8 messageId) const;
private:
    quint16 CalculateInt(double fvcoFreq) const;
    quint32 CalculateFractNew(double fvcoFreq) const;
    quint32 CalculateFractOld(double fvcoFreq) const;
    quint8 CalculateGain(double gain) const;
    quint8 CalculateAttenuator(quint16 attenuator) const;
    bool CalculateDiv(double fvcoFreq) const;
private:
    const double m_c=299792458.0;
    const double m_f;
    const double m_fref;
    const QVarLengthArray<quint8, 8> m_messagesIds={0,1,2,3,4,5,6,7};
};

#endif // MESSAGECREATORS_STATEMESSAGESENDER_H
