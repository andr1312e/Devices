#include "statemessagegetter.h"


StateMessageGetter::StateMessageGetter(QObject *parent)
    : QObject(parent)
    , m_indexInByteArrayOfGettingMessageId(1)
    , m_maxWorkModeIndex(5)
{
    m_repository=new UstrirovMessageRepository();
}

StateMessageGetter::~StateMessageGetter()
{
    delete m_repository;
}

bool StateMessageGetter::FillDataIntoStructFromMessage(const QByteArray &message)
{
    quint8 sendedMessageId=message.at(m_indexInByteArrayOfGettingMessageId);
     qDebug()<< "GET id "<<sendedMessageId << " GET "<<message.toHex();;
    switch (sendedMessageId) {
    case 1:
        return GetFvcoFromFirstMessage(message);
    case 2:
        return GetDoplerFromSecondMessage(message);
    case 3:
        GetDistanceFromThirdMessage(message);
    case 4:
        return GetGainTxGainRXFromFourthMessage(message);
    case 5:
        return GetAttenuatorRXFromFiveMessage(message);
    case 6:
        return GetWorkModeFromSixMessage(message);
    default:
        Q_UNREACHABLE();
    }
}

void StateMessageGetter::SetBadState()
{
    m_repository->ResetRepository();
    m_repository->SetBadState();
}

DevicesAdjustingKitMessage &StateMessageGetter::GetMessage()
{
    return m_repository->GetMessage();
}

bool StateMessageGetter::GetFvcoFromFirstMessage(const QByteArray &message)
{
    if(message.count()==8)
    {
        bool DIV_RX=message.at(7);
        QByteArray arrayINTRX(2, ' ');
        arrayINTRX[0] = message.at(2);
        arrayINTRX[1] = message.at(3);
        QDataStream ds(arrayINTRX);
        quint16 INT_RX=0;
        ds >> INT_RX;
        quint32 INT_RX_BIG=INT_RX;
        INT_RX_BIG=INT_RX_BIG+4;
        INT_RX_BIG=INT_RX_BIG*Fref;
        INT_RX_BIG=INT_RX_BIG*qPow(2,DIV_RX);
        INT_RX_BIG=INT_RX_BIG/2;
        INT_RX_BIG=INT_RX_BIG+3000000.0;
        INT_RX_BIG=INT_RX_BIG/1000000;
        m_repository->SetFvco(INT_RX_BIG);
        return true;
    }
    return false;
}

bool StateMessageGetter::GetDoplerFromSecondMessage(const QByteArray &message)
{
    quint32 currentFvco=m_repository->GetMessage().Fvco;
    if(currentFvco==0)
    {
           m_repository->SetDopler(0);
           qFatal("ddd");
    }
    else
    {
        if(message.count()==8)
        {
            QByteArray arrayINTTX;
            arrayINTTX.append(message.at(2));
            arrayINTTX.append(message.at(3));
            QDataStream IntDataStream(arrayINTTX);
            quint16 INT_TX;
            IntDataStream >> INT_TX;

            QByteArray arrayFRACTRX;
            arrayFRACTRX.append(static_cast<char>(0x00));//иначе будет 0 нужно 4 байта
            arrayFRACTRX.append(message.at(4));
            arrayFRACTRX.append(message.at(5));
            arrayFRACTRX.append(message.at(6));

            QDataStream fractDataStream(arrayFRACTRX);
            quint32 FRACT_RX;
            fractDataStream >> FRACT_RX;

            bool DIV_RX=message.at(7);

            //Значение сидит только здесь, первую парсить не нужно

            double FRACT_RX_BIG=FRACT_RX;
            double pow=qPow(2, 20);
            FRACT_RX_BIG=FRACT_RX_BIG/pow;
            FRACT_RX_BIG=FRACT_RX_BIG+INT_TX+4.0;
            FRACT_RX_BIG=FRACT_RX_BIG/2.0;
            FRACT_RX_BIG=FRACT_RX_BIG*Fref*qPow(2, DIV_RX);
            FRACT_RX_BIG=FRACT_RX_BIG+3000000;
            m_repository->SetDopler(FRACT_RX_BIG-currentFvco*1000000);
            return true;
        }
        return false;
    }
}

bool StateMessageGetter::GetDistanceFromThirdMessage(const QByteArray &message)
{
    return true;
}

bool StateMessageGetter::GetGainTxGainRXFromFourthMessage(const QByteArray &message)
{

    if (message.count()==4)
    {
        quint8 GAIN_TX=message.at(2)*2.0;
        quint8 GAIN_RX=message.at(3)*2.0;
        m_repository->SetGainRx(GAIN_RX);
        m_repository->SetGainTx(GAIN_TX);
        return true;
    }
    return false;
}

bool StateMessageGetter::GetAttenuatorRXFromFiveMessage(const QByteArray &message)
{
    if (message.count()==3)
    {
        quint8 Attenuator_RX=quint8(message.at(2));
        m_repository->SetAttenuator(Attenuator_RX);
        return true;
    }
    return false;
}

bool StateMessageGetter::GetWorkModeFromSixMessage(const QByteArray &message)
{
    if (message.count()==3)
    {
        quint8 WorkModeIndex=quint8(message.at(2));
        if(WorkModeIndex<m_maxWorkModeIndex)
        {
            m_repository->SetWorkMode(WorkModeIndex);
            Q_EMIT ToAllDataCollected();
            return true;
        }

    }
    return false;
}
