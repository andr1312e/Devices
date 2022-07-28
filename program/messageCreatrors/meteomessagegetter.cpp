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
    if (255 == quint8(message.at(2)) && 255 == quint8(message.at(3))) // отказ метеостанции от исполнения команды, при первом запуске миниту может быть это норма
    {
        m_repository->SetDeclineState(); // отказ метеостанции от исполнения команды
        Q_EMIT ToAllDataCollected();
    }
    else
    {
        QByteArray floatBytes = message.mid(6, 4);
        float variable;
        memcpy(&variable, floatBytes.data(), sizeof(float));
        METEO meteoType = static_cast<METEO>(message.at(4));
        switch (meteoType)
        {
        case METEO::METEO_PRESSURE:
        {
            m_repository->ResetRepository();
            m_repository->SetPressure(variable);
            break;
        }
        case METEO::METEO_TEMPERATURE:
        {
            m_repository->SetTemperature(variable);
            break;
        }
        case METEO::METEO_WET:
        {
            m_repository->SetWet(variable);
            m_repository->SetGoodState();
            Q_EMIT ToAllDataCollected();
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

const DevicesMeteoKitGetMessage &MeteoMessageGetter::MessageTimeOut() noexcept
{
    m_repository->SetTimeoutState();
    return m_repository->GetMessage();
}

const DevicesMeteoKitGetMessage &MeteoMessageGetter::NoConnectionMessage() noexcept
{
    m_repository->SetNoConnectionState();
    return m_repository->GetMessage();
}

const DevicesMeteoKitGetMessage &MeteoMessageGetter::GetMessage() noexcept
{
    return m_repository->GetMessage();
}

std::string MeteoMessageGetter::GetLastMessageTime() const noexcept
{
    return m_repository->GetLastMessageTime();
}
