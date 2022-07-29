#include "ustirovmessagegegetter.h"

UstirovMessageGetter::UstirovMessageGetter(const double f, const double fref, UstrirovMessageRepository &messageRepository, GeoMessageRepository &geoRepository, QObject *parent)
    : QObject(parent)
    , m_indexInByteArrayOfGettingMessageId(1)
    , m_f(f)
    , m_Fref(fref)
    , m_countOfWorkModes(5)
    , m_messageRepository(messageRepository)
    , m_geoMessageRepository(geoRepository)
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
        return SaveFvcoTxToRepository(message);
    case 3:
        return SaveDistanceToRepository(message);
    case 4:
        return SaveGainsToRepository(message);
    case 5:
        return SaveAttenuatorToRepository(message);
    case 6:
        return SaveWorkModeToRepository(message);
    case 9:
        return SaveDoplerToRepository(message);
    default:
        throw std::runtime_error("Ид сообщения не обрабатываем");
    }
}

void UstirovMessageGetter::SetNoConnectionStateNormal()
{
    m_messageRepository.SetNoConnectionStateNormal();
}

void UstirovMessageGetter::SetTimeOutState()
{
    m_messageRepository.SetTimeOutStateNormal();
}

const DevicesAdjustingKitMessage &UstirovMessageGetter::GetMessageNormal()
{
    return m_messageRepository.GetNormalMessage();
}

const DevicesBparAdjustingKitMessage &UstirovMessageGetter::GetMessageBpar()
{
    return m_messageRepository.GetBparMessage();
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
        FRACT_RX = FRACT_RX * m_Fref * qPow(2, DIV_RX) + 22;

        m_messageRepository.SetNormalFvcoRx(FRACT_RX + 3000000.0); //поправка на 3

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
        FRACT_TX = FRACT_TX * m_Fref * qPow(2, DIV_TX) + 22;

        m_messageRepository.SetNormalFvcoTx((quint32)FRACT_TX + 3000000.0); //поправка на 3
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
        messageDistance = messageDistance + m_messageRepository.GetDistanceToLocator();
        messageDistance += 17.9434889941;
        m_messageRepository.SetNormalDistance(qCeil(messageDistance));
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
        m_messageRepository.SetNormalGainRx(GAIN_RX);
        m_messageRepository.SetNormalGainTx(GAIN_TX);
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
        m_messageRepository.SetNormalAttenuator(realValue);
        return true;
    }
    return false;
}

bool UstirovMessageGetter::SaveWorkModeToRepository(const QByteArray &message)
{
    if (8 == message.count() && quint8(message.at(2) == 5))
    {
        const bool canBparParce = SaveBparToRepository(message);
        if (canBparParce)
        {
            Q_EMIT ToAllBparDataCollected();
        }
        return canBparParce;
    }
    else
    {
        if (3 == message.count())
        {
            const quint8 WorkModeIndex = quint8(message.at(2));
            if (WorkModeIndex < m_countOfWorkModes)
            {
                m_messageRepository.SetNormalWorkMode(WorkModeIndex);
                m_messageRepository.SetNormalCompleteState();
                Q_EMIT ToAllNormalDataCollected();
                return true;
            }
        }
    }
    return false;
}


bool UstirovMessageGetter::SaveDoplerToRepository(const QByteArray &message)
{
    if (message.count() == 5)
    {
        QByteArray arrayDopler;
        arrayDopler.append(static_cast<char>(0x00));//иначе будет 0, нужно 4 байта
        arrayDopler.append(message.at(2));
        arrayDopler.append(message.at(3));
        arrayDopler.append(message.at(4));

        QDataStream doplerDataStream(arrayDopler);
        quint32 dopler;
        doplerDataStream >> dopler;

        m_messageRepository.SetNormalDopler(dopler);
        return true;
    }
    return false;
}

void UstirovMessageGetter::SaveCoordinatesToRepository(const QByteArray &message)
{
    const int indexOfHeader = message.indexOf(QByteArrayLiteral("$GNR"));
    if (-1 == indexOfHeader)
    {
        if (m_hasGeoHeader)
        {
            int indexOfAsterisk = message.indexOf('*');
            if (indexOfAsterisk == -1)
            {
                m_collectedGeoData.append(message);

            }
            else
            {
                m_collectedGeoData.append(message.left(indexOfAsterisk));
                m_hasGeoHeader = false;
                ParceGeoMessage();
            }
        }
    }
    else
    {
        m_hasGeoHeader = true;
        m_collectedGeoData.append(message.left(indexOfHeader + 1));
        const int indexOfAsterisk = message.indexOf('*', indexOfHeader);
        if (-1 == indexOfAsterisk)
        {
            m_collectedGeoData = message.mid(indexOfHeader);
        }
        else
        {
            m_collectedGeoData = message.mid(indexOfHeader, indexOfAsterisk - indexOfHeader + 1);
            m_hasGeoHeader = false;
            ParceGeoMessage();
        }
    }
}

bool UstirovMessageGetter::SaveBparToRepository(const QByteArray &message)
{
    if (message.count() == 8)
    {
        if (message.at(2) == 5)
        {
            const quint8 bpar_mode = message.at(3);
            //3 бита
            //было  11000000
            //стало 00000110
            //прибавляем к результату 1 так как смотри таблицу, там первый начинается с 0
            const quint8 foId = (bpar_mode & 7);

            //позиция начинается справа
            //было  11011000  = 216
            //маска 00010000  = 16
            //сдвигаем на 4 разряда и получаем или 0 или 1
            const quint8 hasLcmMask = GetMask(3, sizeof(  quint8 ) );
            const bool isLcm = ( bpar_mode & hasLcmMask ) >> 3;
            //позиция начинается справа
            //было  11011000  = 216
            //маска 00001100  = 12
            //сдвигаем на 4 разряда и получаем или 2 битовое число
            quint8 tksIndex = 0;
            if (!isLcm)
            {
                const quint8 tKMask = GetMask(4, sizeof(  quint8 ) * 2 );
                tksIndex = ( bpar_mode & tKMask ) >> 4;
            }
            QByteArray signalDelayArray;
            signalDelayArray.append(message.at(4));
            signalDelayArray.append(message.at(5));
            QDataStream sinusDataStream(signalDelayArray);
            quint16 signalDelay;
            sinusDataStream >> signalDelay;
            const quint32 answerDelay = ParceDelay(isLcm, signalDelay);

            QByteArray thresholdArray;
            thresholdArray.append(message.at(6));
            thresholdArray.append(message.at(7));
            QDataStream thresholdDataStream(thresholdArray);
            quint16 threshold;
            thresholdDataStream >> threshold;

            m_messageRepository.SetBpar(foId, isLcm, tksIndex, answerDelay, threshold);
            return true;
        }
    }
    return false;
}

quint8 UstirovMessageGetter::GetMask(quint8 pos, quint8 size) const noexcept
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
    Q_ASSERT(message.count() > 7);
    const bool DIV = message.at(7);
    return DIV;
}

quint32 UstirovMessageGetter::GetDistanceFromMessage(const QByteArray &message) const
{
    Q_ASSERT(message.count() == 4);
    QByteArray distanceArray;
    distanceArray.append(message.at(2));
    distanceArray.append(message.at(3));
    QDataStream IntDataStream(distanceArray);
    quint16 distance;
    IntDataStream >> distance;
    return distance;
}

quint32 UstirovMessageGetter::ParceDelay(bool isLcm, quint16 delay) const
{
    delay -= 1;
    const double secondVal = m_c / m_f;
    double distanceDouble = secondVal * (delay) / 2.0;
    distanceDouble = distanceDouble + m_messageRepository.GetDistanceToLocator();
    if (isLcm)
    {
        distanceDouble = qAbs(distanceDouble - 117.488879661);
    }
    else
    {
        distanceDouble = qAbs(distanceDouble - 176.233319492);
    }
    return distanceDouble;
}

void UstirovMessageGetter::ParceGeoMessage()
{
//    m_geoMessageRepository->ResetMessage();
//    qDebug() << m_collectedKoordinatesData;
    const int commaCount = m_collectedGeoData.count(',');
    if (13 == commaCount)
    {
        QVector<int> indexOfCommasArray{m_collectedGeoData.indexOf(',')};
        for (int i = 1; i < commaCount; ++i)
        {
            indexOfCommasArray.append(m_collectedGeoData.indexOf(',', indexOfCommasArray.at(i - 1) + 1));
        }
        if (indexOfCommasArray.at(3) == indexOfCommasArray.at(4) - 2)
        {
            const QByteArray latitudeByteArray = m_collectedGeoData.mid(indexOfCommasArray.at(2) + 1, indexOfCommasArray.at(3) - indexOfCommasArray.at(2) - 1);
            const char latitudeDirection = m_collectedGeoData.at(indexOfCommasArray.at(4) - 1);
            const int directionValue = latitudeDirection == 'N' ? 1 : -1;
            m_geoMessageRepository.SetLatitude(ParceLatitdeValue(latitudeByteArray, directionValue));
        }
        else
        {
            m_geoMessageRepository.SetWrongMessageState();
        }


        if (indexOfCommasArray.at(5) == indexOfCommasArray.at(6) - 2)
        {
            const QByteArray longtutudeByteArray = m_collectedGeoData.mid(indexOfCommasArray.at(4) + 1, indexOfCommasArray.at(5) - indexOfCommasArray.at(4) - 1);
            const char longtutudeDirection = m_collectedGeoData.at(indexOfCommasArray.at(6) - 1);
            const int directionValue = longtutudeDirection == 'E' ? 1 : -1;
            m_geoMessageRepository.SetLongitutude(ParceLongtitude(longtutudeByteArray, directionValue));
        }
        else
        {
            m_geoMessageRepository.SetWrongMessageState();
        }
        m_collectedGeoData.clear();
        Q_EMIT ToAllGeoCollected();
    }
}

double UstirovMessageGetter::ParceLatitdeValue(const QByteArray &latitudeByteArray, int directionValue) const
{
    if (10 == latitudeByteArray.count())
    {
        //"ГГММ.МММММ". Например  "5546.95900" = 55°46.959' = 55.78265°.
        //ГГ - градусы; ММ.ММММ - минуты и десятитысячные доли минут.
        //Для ГГММ.ММММ → ГГ.ГГГГГГ перевода необходимо использовать формулу ГГ + (ММ.ММММ ÷ 60):
        const char gradusesFirst = latitudeByteArray.front();
        const char gradusesSecond = latitudeByteArray.at(1);
        const int graduses = (gradusesFirst - 48) * 10.0 + (gradusesSecond - 48);
        const char minutesDecimalFirst = latitudeByteArray.at(2);
        const char minutesDecimalSecond = latitudeByteArray.at(3);
        const char minutesFractFirst = latitudeByteArray.at(5);
        const char minutesFractSecond = latitudeByteArray.at(6);
        const char minutesFractThird = latitudeByteArray.at(7);
        const char minutesFractFourth = latitudeByteArray.at(8);
        const char minutesFractFive = latitudeByteArray.at(9);
        const double minutes = (minutesDecimalFirst - 48.0) * 10.0 + (minutesDecimalSecond - 48.0)
                               + (minutesFractFirst - 48.0) / 10.0 + (minutesFractSecond - 48.0) / 100.0f
                               + (minutesFractThird - 48.0) / 1000.0 + (minutesFractFourth - 48.0) / 10000.0f +
                               (minutesFractFive - 48.0) / 100000.0;
        //    qDebug()<< "ParceLatitde " << graduses << "   " << minutes;
        const double realCoordinate =  directionValue * (graduses + minutes / 60.0);
        return realCoordinate;
    }
    else
    {
        return 0.0;
    }
}

double UstirovMessageGetter::ParceLongtitude(const QByteArray &longtitudeByteArray, int directionValue) const
{
    if (11 == longtitudeByteArray.count())
    {
        const char gradusesFirst = longtitudeByteArray.front();
        const char gradusesSecond = longtitudeByteArray.at(1);
        const char gradusesThird = longtitudeByteArray.at(2);
        const int graduses = (gradusesFirst - 48) * 100 + (gradusesSecond - 48) * 10 + (gradusesThird - 48);
        const char minutesDecimalFirst = longtitudeByteArray.at(3);
        const char minutesDecimalSecond = longtitudeByteArray.at(4);
        const char minutesFractFirst = longtitudeByteArray.at(6);
        const char minutesFractSecond = longtitudeByteArray.at(7);
        const char minutesFractThird = longtitudeByteArray.at(8);
        const char minutesFractFourth = longtitudeByteArray.at(9);
        const char minutesFractFive = longtitudeByteArray.at(10);
        const float minutes = (minutesDecimalFirst - 48.0) * 10.0 + (minutesDecimalSecond - 48.0)
                              + (minutesFractFirst - 48.0) / 10.0 + (minutesFractSecond - 48.0) / 100.0
                              + (minutesFractThird - 48.0) / 1000.0 + (minutesFractFourth - 48.0) / 10000.0 +
                              (minutesFractFive - 48.0) / 100000.0;
        const double realCoordinate = directionValue * graduses + minutes / 60.0;
        return realCoordinate;
    }
    else
    {
        return 0.0;
    }
}
