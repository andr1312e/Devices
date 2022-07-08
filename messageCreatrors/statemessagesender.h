#ifndef MESSAGECREATORS_STATEMESSAGESENDER_H
#define MESSAGECREATORS_STATEMESSAGESENDER_H

#include <QByteArray>
#include <QIODevice>
#include <QDataStream>
#include <QVarLengthArray>
#include <QSharedPointer>
#include <cmath>

#include "constants.h"
#include "mediators/logger.h"
#include "messageRepositories/ustrirovmessagerepository.h"

class UstirovMessageSender
{
public:
    explicit UstirovMessageSender(const Logger *logger, const double f, const double fref);
    ~UstirovMessageSender();
public:
    QByteArray CreateRestartCommand() const noexcept;
    QByteArray CreateZeroCommand() const noexcept;
    QByteArray CreateFirstCommand(double fvcoFreq) const noexcept;
    QByteArray CreateSecondCommand(double fvcoFreq) const noexcept;
    QByteArray CreateThirdCommand(double distance, double distanceToLocator) const noexcept;
    QByteArray CreateFourthCommand(double gainTX, double gainRX) const  noexcept;
    QByteArray CreateFiveCommand(double attenuator) const;
    QByteArray CreateSixCommand(quint8 workMode) const noexcept;
    QByteArray CreateSevenCommand(quint8 messageId) const noexcept;
    QByteArray CreateNineCommand(quint32 dopler) const noexcept;
    QByteArray CreateBparCommand(const DevicesBparAdjustingKitMessage &message) const noexcept;
private:
    quint16 CalculateInt(double fvcoFreq) const;
    quint32 CalculateFractNew(double fvcoFreq) const;
    quint32 CalculateFractOld(double fvcoFreq) const;
    quint8 CalculateGain(double gain) const noexcept;
    quint8 CalculateAttenuator(quint16 attenuator) const;
    bool CalculateDiv(double fvcoFreq) const noexcept;
    quint16 CalculateBparDistance(double answerDelay, quint32 distanceToSolver, bool isLcm) const;
private:
    const Logger *const m_logger;
    const double m_c = 299792458.0;
    const double m_f;
    const double m_fref;
    const QVarLengthArray<quint8, 8> m_messagesIds = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
};

#endif // MESSAGECREATORS_STATEMESSAGESENDER_H
