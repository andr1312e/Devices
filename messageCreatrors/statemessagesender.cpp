#include "statemessagesender.h"

UstirovMessageSender::UstirovMessageSender(const Logger *logger, const double f, const double fref)
    : m_logger(logger)
    , m_f(f)
    , m_fref(fref)
{

}

UstirovMessageSender::~UstirovMessageSender()
{

}

QByteArray UstirovMessageSender::CreateRestartCommand() const noexcept
{
    return QByteArray(QByteArrayLiteral("\x0d\x5a"));
}

QByteArray UstirovMessageSender::CreateZeroCommand() const noexcept
{
    QByteArray command;
    command.append(m_messagesIds.at(0));
    command.append(m_messagesIds.at(0));
    return command;
}

QByteArray UstirovMessageSender::CreateFirstCommand(double fvcoFreq) const noexcept
{
    const quint8 id = m_messagesIds.at(1);
    const qint16 intRx = CalculateInt(fvcoFreq); //МГЦ
    const qint32 fractRx = CalculateFractNew(fvcoFreq); //МГЦ
    const bool divRx = CalculateDiv(fvcoFreq);


    QByteArray lastThreeBytes;
    QDataStream threeBytesStream(&lastThreeBytes, QIODevice::WriteOnly);
    threeBytesStream << fractRx;


    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << id;
    streamMain << intRx;
    streamMain << (quint8)lastThreeBytes.at(1);
    streamMain << (quint8)lastThreeBytes.at(2);
    streamMain << (quint8)lastThreeBytes.at(3);
    streamMain << divRx;
    return command;
}

QByteArray UstirovMessageSender::CreateSecondCommand(double fvcoFreq) const noexcept
{
    const qint16 intTx = CalculateInt(fvcoFreq);
    const qint32 fractTx = CalculateFractNew(fvcoFreq);
    const bool divTx = CalculateDiv(fvcoFreq);


    QByteArray lastThreeBytes;
    QDataStream threeBytesStream(&lastThreeBytes, QIODevice::WriteOnly);
    threeBytesStream << fractTx;

    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << m_messagesIds.at(2);
    streamMain << intTx;
    streamMain << (quint8)lastThreeBytes.at(1);
    streamMain << (quint8)lastThreeBytes.at(2);
    streamMain << (quint8)lastThreeBytes.at(3);
    streamMain << divTx;

    return command;
}

QByteArray UstirovMessageSender::CreateThirdCommand(double distance, double distanceToLocator) const noexcept
{
    distance -= 26, 9434889941;
    const double secondVal = m_f / m_c;
    //    m_logger->Appends("!!!!!Дистанция f/c= " + std::to_string(secondVal));
    const double newDistance = 2.0 * qAbs(distance - distanceToLocator) * secondVal + 1.0;
    //    m_logger->Appends("!!!!!Дистанция - дист до локатора " + std::to_string(newDistance));
    quint16 DISTANCE = newDistance;
    //    m_logger->Appends("!!!!!Дистанция - в сообщение пишем " + std::to_string(DISTANCE));
    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << m_messagesIds.at(3);
    streamMain << DISTANCE;
    return command;
}

QByteArray UstirovMessageSender::CreateFourthCommand(double gainTX, double gainRX) const noexcept
{
    const quint8 GAIN_TX = CalculateGain(gainTX);
    const quint8 GAIN_RX = CalculateGain(gainRX);

    QByteArray command;
    command.append(m_messagesIds.at(4));
    command.append(GAIN_TX);
    command.append(GAIN_RX);
    return command;
}

QByteArray UstirovMessageSender::CreateFiveCommand(double attenuator) const
{
    quint8 attenuatorInt = CalculateAttenuator(attenuator);

    QByteArray command;
    command.append(m_messagesIds.at(5));
    command.append(attenuatorInt);
    return command;
}

QByteArray UstirovMessageSender::CreateSixCommand(quint8 workMode) const noexcept
{
    QByteArray command;
    command.append(m_messagesIds.at(6));
    command.append(workMode);
    if (workMode > 2)
    {
        quint32 sinusVal = 0;
        const quint8 first = (sinusVal >> (8 * 0)) & 0xff;
        const quint8 second = (sinusVal >> (8 * 1)) & 0xff;
        const quint8 third = (sinusVal >> (8 * 2)) & 0xff;
        command.append(third);
        command.append(second);
        command.append(first);
    }
    return command;
}

QByteArray UstirovMessageSender::CreateSevenCommand(quint8 messageId) const noexcept
{
    QByteArray command;
    command.append(m_messagesIds.at(7));
    command.append(quint8(messageId));
    return command;
}

QByteArray UstirovMessageSender::CreateNineCommand(quint32 dopler) const noexcept
{
    const quint8 id = m_messagesIds.at(9);

    QByteArray lastThreeBytes;
    QDataStream threeBytesStream(&lastThreeBytes, QIODevice::WriteOnly);
    threeBytesStream << dopler;

    QByteArray command;
    QDataStream streamMain(&command, QIODevice::WriteOnly);
    streamMain << id;
    streamMain << (quint8)lastThreeBytes.at(1);
    streamMain << (quint8)lastThreeBytes.at(2);
    streamMain << (quint8)lastThreeBytes.at(3);
    return command;
}

QByteArray UstirovMessageSender::CreateBparCommand(const DevicesBparAdjustingKitMessage &message) const  noexcept
{
    QByteArray command;
    QDataStream stream(&command, QIODevice::WriteOnly);
    quint8 bpar_mode=message.foId;
    if (message.isLcm)
    {
        const quint8 lcmMode = 8; //1000
        bpar_mode += lcmMode;
    }
    else
    {
        const quint8 tkVal = message.tksIndex << 4;//10000 или 20000
        bpar_mode += tkVal;
    }
    if (message.hasThreshold)
    {
        bpar_mode += 64;//0100000
    }
    const quint16 delay = CalculateBparDistance(message.answerDelay, message.DistanceToLocator);
    stream << (quint8)6;
    stream << (quint8)5;
    stream << bpar_mode;
    stream << delay;
    stream << message.threshold;
    return command;
}

quint16 UstirovMessageSender::CalculateInt(double fvcoFreq) const
{
    const bool DIV_Rx = CalculateDiv(fvcoFreq);
    double INT_RxDouble = fvcoFreq - 3000000.0;
    INT_RxDouble = INT_RxDouble * 2.0;
    INT_RxDouble = INT_RxDouble / pow(2, DIV_Rx);
    INT_RxDouble = (INT_RxDouble / m_fref);
    quint16 INT_Rx = (quint16)(INT_RxDouble);
    INT_Rx -= 4;
    return INT_Rx;
}

quint32 UstirovMessageSender::CalculateFractNew(double fvcoFreq) const
{
    const bool DIV_Rx = CalculateDiv(fvcoFreq);
    double FRACT_Rx = (pow(2, 20));
    fvcoFreq = fvcoFreq - 3000000.0;
    double FirstValue = 2.0 * fvcoFreq;
    double FirstValueDiv = m_fref * pow(2, DIV_Rx);
    FirstValue = FirstValue / FirstValueDiv;
    double SecondValue = (qint32)FirstValue;
    FRACT_Rx = FRACT_Rx * (FirstValue - SecondValue);
    return FRACT_Rx;
}

quint32 UstirovMessageSender::CalculateFractOld(double fvcoFreq) const
{
    const bool DIV_Rx = CalculateDiv(fvcoFreq);
    double FRACT_Rx = (pow(2, 20));
    double FirstValue = 2.0 * fvcoFreq;
    const  double FirstValueDiv = m_fref * pow(2, DIV_Rx);
    FirstValue = FirstValue / FirstValueDiv;
    const double SecondValue = (qint32)FirstValue;
    FRACT_Rx = FRACT_Rx * (FirstValue - SecondValue);
    return FRACT_Rx;
}

quint8 UstirovMessageSender::CalculateGain(double gain) const noexcept
{
    if (gain > (31.5))
    {
        gain = 31.5;
    }
    const quint8 GAIN_X = gain * 2.0;
    return GAIN_X;
}

quint8 UstirovMessageSender::CalculateAttenuator(quint16 attenuator) const
{
    if (atteniatorTable.contains(attenuator))
    {
        const quint8 atteniatorValue = atteniatorTable[attenuator];
        return atteniatorValue;
    }
    else
    {
        qFatal("нет значения в таблице");
    }
}

bool UstirovMessageSender::CalculateDiv(double fvcoFreq) const noexcept
{
    if (fvcoFreq > 2750000000)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

quint16 UstirovMessageSender::CalculateBparDistance(int answerDelay, quint32 distanceToSolver) const
{
    return (answerDelay-distanceToSolver)/m_f*m_c;
}
