#include "meteomessagegetter.h"
#include <QDebug>

MeteoMessageGetter::MeteoMessageGetter(QObject *parent)
    : QObject(parent)
    , m_repository(new MeteoMessageRepository())
{

}

MeteoMessageGetter::~MeteoMessageGetter()
{
    delete m_repository;
}

void MeteoMessageGetter::ParseMessage(const QByteArray &message)
{
    if(255==quint8(message.at(2)) && 255 == quint8(message.at(3))) // отказ метеостанции от исполнения команды, при первом запуске миниту может быть это норма
    {
        m_repository->SetDeclineState(); // отказ метеостанции от исполнения команды
        Q_EMIT ToAllDataCollected();
    }
    else
    {
        QByteArray floatBytes = message.mid(6, 4);
        float variable;
        memcpy(&variable, floatBytes.data(), sizeof(float));

        switch(message.at(4))
        {
        case METEO_PRESSURE:
        {
            m_repository->ResetRepository();
            m_repository->SetPressure(variable);
//            qDebug()<< "Метео : Давление " << variable;
            break;
        }
        case METEO_TEMPERATURE:
        {
            m_repository->SetTemperature(variable);
//            qDebug()<< "Метео : Температура " << variable;
            break;
        }
        case METEO_WET:
        {
            m_repository->SetWet(variable);
            m_repository->SetGoodState();
            Q_EMIT ToAllDataCollected();
//            qDebug()<< "Метео : влажность " << variable;
            break;
        }
        default:
        {
            m_repository->SetWrongData();
            Q_EMIT ToAllDataCollected();
            break;
        }
        }
    }
}

const DevicesMeteoKitGetMessage &MeteoMessageGetter::MessageTimeOut()
{
    m_repository->SetTimeoutState();
    return m_repository->GetMessage();
}

const DevicesMeteoKitGetMessage &MeteoMessageGetter::NoConnectionMessage()
{
    m_repository->SetNoConnectionState();
    return m_repository->GetMessage();
}

const DevicesMeteoKitGetMessage &MeteoMessageGetter::GetMessage() const
{
    return m_repository->GetMessage();
}

const std::string MeteoMessageGetter::GetLastMessageTime() const
{
    return m_repository->GetLastMessageTime();
}
