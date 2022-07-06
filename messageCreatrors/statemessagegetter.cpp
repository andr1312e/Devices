#include "statemessagegetter.h"


UstirovMessageGetter::UstirovMessageGetter(const double f, const double fref, UstrirovMessageRepository *messageRepository, QObject *parent)
    : QObject(parent)
    , m_indexInByteArrayOfGettingMessageId(1)
    , m_f(f)
    , m_Fref(fref)
    , m_countOfWorkModes(5)
    , m_messageRepository(messageRepository)
{

}

UstirovMessageGetter::~UstirovMessageGetter()
{

}

bool UstirovMessageGetter::FillDataIntoStructFromMessage(const QByteArray &message)
{
    const quint8 sendedMessageId = message.at(m_indexInByteArrayOfGettingMessageId);
    switch (sendedMessageId)
    {
    case 1:
        return SaveFvcoRxToRepository(message);
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

void UstirovMessageGetter::SetNoConnectionStateNormal()
{
    m_messageRepository->SetNoConnectionStateNormal();
}

void UstirovMessageGetter::SetTimeOutState()
{
    m_messageRepository->SetTimeOutStateNormal();
}

const DevicesAdjustingKitMessage &UstirovMessageGetter::GetMessageNormal()
{
    return m_messageRepository->GetNormalMessage();
}

const DevicesBparAdjustingKitMessage &UstirovMessageGetter::GetMessageBpar()
{
    return m_messageRepository->GetBparMessage();
}

bool UstirovMessageGetter::SaveFvcoRxToRepository(const QByteArray &message)
{
    if (8 == message.count())
    {
        const quint16 INT_RX = GetIntFromMessage(message);
        double FRACT_RX = (double)GetFractFromMessage(message);
        const bool DIV_RX = GetDivFromMessage(message);

        //Значение сидит только здесь, первую парсить не нужно

        const double pow = qPow(2, 20);
        FRACT_RX = FRACT_RX / pow;
        FRACT_RX = FRACT_RX + INT_RX + 4.0;
        FRACT_RX = FRACT_RX / 2.0;
        FRACT_RX = FRACT_RX * m_Fref * qPow(2, DIV_RX);

        m_messageRepository->SetNormalFvcoRx((quint32)FRACT_RX + 3000000.0); //поправка на 3
        return true;
    }
    return false;
}

bool UstirovMessageGetter::SaveFvcoTxToRepository(const QByteArray &message)
{
    if (8 == message.count())
    {
        const quint16 INT_TX = GetIntFromMessage(message);
        double FRACT_TX = (double)GetFractFromMessage(message);
        const bool DIV_TX = GetDivFromMessage(message);

        //Значение сидит только здесь, первую парсить не нужно

        const double pow = qPow(2, 20);
        FRACT_TX = FRACT_TX / pow;
        FRACT_TX = FRACT_TX + INT_TX + 4.0;
        FRACT_TX = FRACT_TX / 2.0;
        FRACT_TX = FRACT_TX * m_Fref * qPow(2, DIV_TX);

        m_messageRepository->SetNormalFvcoTx((quint32)FRACT_TX + 3000000.0); //поправка на 3
        return true;
    }
    return false;
}

bool UstirovMessageGetter::SaveDistanceToRepository(const QByteArray &message)
{
    if (4 == message.count())
    {
        double messageDistance = (double)GetDistanceFromMessage(message);
        messageDistance = messageDistance / 2.0 * m_c / m_f;
        messageDistance = messageDistance + m_messageRepository->GetDistanceToLocator();
        m_messageRepository->SetNormalDistance(qCeil(messageDistance));
        return true;
    }
    return false;
}

bool UstirovMessageGetter::SaveGainsToRepository(const QByteArray &message)
{

    if (4 == message.count())
    {
        const float GAIN_TX = message.at(2) / 2.0;
        const float GAIN_RX = message.at(3) / 2.0;
        m_messageRepository->SetNormalGainRx(GAIN_RX);
        m_messageRepository->SetNormalGainTx(GAIN_TX);
        return true;
    }
    return false;
}

bool UstirovMessageGetter::SaveAttenuatorToRepository(const QByteArray &message)
{
    if (3 == message.count())
    {
        const quint8 Attenuator_RX = quint8(message.at(2));
        const quint8 realValue = atteniatorTable.key(Attenuator_RX);
        m_messageRepository->SetNormalAttenuator(realValue);
        return true;
    }
    return false;
}

bool UstirovMessageGetter::SaveWorkModeToRepository(const QByteArray &message)
{
    if (3 == message.count())
    {
        const quint8 WorkModeIndex = quint8(message.at(2));
        if (WorkModeIndex < m_countOfWorkModes)
        {
            m_messageRepository->SetNormalWorkMode(WorkModeIndex);
            m_messageRepository->SetNormalCompleteState();
            Q_EMIT ToAllNormalDataCollected();
            return true;
        }

    }
    else
    {
        if(6== message.count())
        {
            const bool canBparParce=SaveBparToRepository(message);
            if(canBparParce)
            {
                Q_EMIT ToAllBparDataCollected();
            }
            return canBparParce;
        }
    }
    return false;
}


bool UstirovMessageGetter::SaveDoplerToRepository(const QByteArray &message)
{
    if (message.count() == 6)
    {
        QByteArray arrayDopler;
        arrayDopler.append(static_cast<char>(0x00));//иначе будет 0, нужно 4 байта
        arrayDopler.append(message.at(3));
        arrayDopler.append(message.at(4));
        arrayDopler.append(message.at(5));

        QDataStream doplerDataStream(arrayDopler);
        quint32 dopler;
        doplerDataStream >> dopler;

        m_messageRepository->SetNormalDopler(dopler);
        return true;
    }
    return false;
}

bool UstirovMessageGetter::SaveBparToRepository(const QByteArray &message)
{
    const quint8 bpar_mode = message.at(3);

    //было  11000000
    //стало 00000110
    //-1 так как смотри таблицу, там первый начинается с 0
    const quint8 fo = (bpar_mode >> 5) - 1;

    //позиция начинается справа
    //было  11011000  = 216
    //маска 00010000  = 16
    //сдвигаем на 4 разряда и получаем или 0 или 1
    const quint8 hasLcmPos = 4;
    const quint8 hasLcmMask = GetMask(hasLcmPos, sizeof(  quint8 ) );
    const bool hasLcm = ( bpar_mode & hasLcmMask ) >> hasLcmPos;
    //позиция начинается справа
    //было  11011000  = 216
    //маска 00001100  = 12
    //сдвигаем на 4 разряда и получаем или 2 битовое число
    quint8 tKmode = 0;
    if (!hasLcm)
    {
        const quint8 tKmodePos = 2;
        const quint8 tKMask = GetMask(tKmodePos, sizeof(  quint8 ) * 2 );
        tKmode = ( bpar_mode & tKMask ) >> tKmodePos;
    }
    QByteArray signalDelayArray;
    signalDelayArray.append(message.at(4));
    signalDelayArray.append(message.at(5));

    QDataStream sinusDataStream(signalDelayArray);
    quint16 signalDelay;
    sinusDataStream >> signalDelay;

    return true;
}

quint8 UstirovMessageGetter::GetMask(quint8 pos, quint8 size) const
{
    //size - размер числа * колво бит нужно
    //pos - позиция числаы
    return ~( ~0ull << size ) << pos;
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
    const bool DIV = message.at(7);
    return DIV;
}

quint32 UstirovMessageGetter::GetDistanceFromMessage(const QByteArray &message) const
{
    QByteArray distanceArray;
    distanceArray.append(message.at(2));
    distanceArray.append(message.at(3));
    QDataStream IntDataStream(distanceArray);
    quint16 distance;
    IntDataStream >> distance;
    return distance;
}
