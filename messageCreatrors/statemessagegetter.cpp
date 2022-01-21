#include "statemessagegetter.h"


UstirovMessageGetter::UstirovMessageGetter(const double f, const double fref, QSharedPointer<UstrirovMessageRepository> &messageRepository, QObject *parent)
    : QObject(parent)
    , m_indexInByteArrayOfGettingMessageId(1)
    , f(f)
    , Fref(fref)
    , m_countOfWorkModes(5)
    , m_messageRepository(messageRepository)
{

}

UstirovMessageGetter::~UstirovMessageGetter()
{

}

bool UstirovMessageGetter::FillDataIntoStructFromMessage(const QByteArray &message)
{
    const quint8 sendedMessageId=message.at(m_indexInByteArrayOfGettingMessageId);
    switch (sendedMessageId)
    {
    case 1:
        return SaveFvcoToRepository(message);
    case 2:
        return SaveDoplerToRepository(message);
    case 3:
        return SaveDistanceToRepository(message);
    case 4:
        return SaveGainsToRepository(message);
    case 5:
        return SaveAttenuatorToRepository(message);
    case 6:
        return SaveWorkModeToRepository(message);
    default:
        Q_UNREACHABLE();
    }
}

void UstirovMessageGetter::SetNoConnectionState()
{
    m_messageRepository->SetNoConnectionState();
}

void UstirovMessageGetter::SetTimeOutState()
{
    m_messageRepository->SetTimeOutState();
}

DevicesAdjustingKitMessage &UstirovMessageGetter::GetMessage()
{
    return m_messageRepository->GetMessage();
}

bool UstirovMessageGetter::SaveFvcoToRepository(const QByteArray &message)
{
    if(8==message.count())
    {


        const quint16 INT_RX=GetIntFromMessage(message);
        double FRACT_RX=(double)GetFractFromMessage(message);
        const bool DIV_RX=GetDivFromMessage(message);

        //Значение сидит только здесь, первую парсить не нужно

        double pow=qPow(2, 20);
        FRACT_RX=FRACT_RX/pow;
        FRACT_RX=FRACT_RX+INT_RX+4.0;
        FRACT_RX=FRACT_RX/2.0;
        FRACT_RX=FRACT_RX*Fref*qPow(2, DIV_RX);

        m_messageRepository->SetFvco((quint32)FRACT_RX);
         qDebug()<< QStringLiteral("UMG: Get Work Point ") << m_messageRepository->GetFvco();
        return true;
    }
    return false;
}

bool UstirovMessageGetter::SaveDoplerToRepository(const QByteArray &message)
{
    const quint64 fvcoFreq=m_messageRepository->GetFvco();
    if(0==fvcoFreq)
    {
           m_messageRepository->SetDopler(0);
           qFatal("fvco is null but cant be null");
    }
    else
    {
        if(8==message.count())
        {
            const quint16 INT_TX=GetIntFromMessage(message);
            double FRACT_TX=(double)GetFractFromMessage(message);
            const bool DIV_TX=GetDivFromMessage(message);

            //Значение сидит только здесь, первую INT_RX парсить не нужно
            const double pow=qPow(2, 20);
            FRACT_TX=FRACT_TX/pow;
            FRACT_TX=FRACT_TX+INT_TX+4.0;
            FRACT_TX=FRACT_TX/2.0;
            FRACT_TX=FRACT_TX*Fref*qPow(2, DIV_TX);
            m_messageRepository->SetDopler(qCeil(FRACT_TX-fvcoFreq));
            qDebug()<< QStringLiteral("UMG: Get Dopler ") << m_messageRepository->GetMessage().DoplerFrequency;
            return true;
        }
        return false;
    }
}

bool UstirovMessageGetter::SaveDistanceToRepository(const QByteArray &message)
{
    if(4==message.count())
    {
        double messageDistance=(double)GetDistanceFromMessage(message);
        messageDistance=messageDistance/f*c;
        messageDistance=messageDistance+m_messageRepository->GetDistanceToLocator();
        m_messageRepository->SetDistance(qCeil(messageDistance));
        return true;
    }
    return false;
}

bool UstirovMessageGetter::SaveGainsToRepository(const QByteArray &message)
{

    if (4==message.count())
    {
        float GAIN_TX=message.at(2)/2.0;
        float GAIN_RX=message.at(3)/2.0;
        m_messageRepository->SetGainRx(GAIN_RX);
        m_messageRepository->SetGainTx(GAIN_TX);
        return true;
    }
    return false;
}

bool UstirovMessageGetter::SaveAttenuatorToRepository(const QByteArray &message)
{
    if (3==message.count())
    {
        quint8 Attenuator_RX=quint8(message.at(2));
        m_messageRepository->SetAttenuator(Attenuator_RX);
        return true;
    }
    return false;
}

bool UstirovMessageGetter::SaveWorkModeToRepository(const QByteArray &message)
{
    if (3==message.count())
    {
        quint8 WorkModeIndex=quint8(message.at(2));
        if(WorkModeIndex<m_countOfWorkModes)
        {
            m_messageRepository->SetWorkMode(WorkModeIndex);
            Q_EMIT ToAllDataCollected();
            return true;
        }

    }
    return false;
}

quint16 UstirovMessageGetter::GetIntFromMessage(const QByteArray &message) const
{
    QByteArray intArray;
    intArray.append(message.at(2));
    intArray.append(message.at(3));
    QDataStream IntDataStream(intArray);
    quint16 INT;
    IntDataStream >> INT;
    return INT;
}

quint32 UstirovMessageGetter::GetFractFromMessage(const QByteArray &message) const
{
    QByteArray fractArray;
    fractArray.append(static_cast<char>(0x00));//иначе будет 0, нужно 4 байта
    fractArray.append(message.at(4));
    fractArray.append(message.at(5));
    fractArray.append(message.at(6));

    QDataStream fractDataStream(fractArray);
    quint32 FRACT;
    fractDataStream >> FRACT;
    return FRACT;
}

bool UstirovMessageGetter::GetDivFromMessage(const QByteArray &message) const
{
    const bool DIV=message.at(7);
    return DIV;
}

quint16 UstirovMessageGetter::GetDistanceFromMessage(const QByteArray &message) const
{
    QByteArray distanceArray;
    distanceArray.append(message.at(2));
    distanceArray.append(message.at(3));
    QDataStream IntDataStream(distanceArray);
    quint16 distance;
    IntDataStream >>distance;
    return distance;
}
