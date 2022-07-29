#include "meteomessagerepository.h"

MeteoMessageRepository::MeteoMessageRepository()
{
    ResetRepository();
}

MeteoMessageRepository::~MeteoMessageRepository()
{

}

void MeteoMessageRepository::SetPressure(float pressure) noexcept
{
    m_repository.pressure = pressure;
}

void MeteoMessageRepository::SetTemperature(float temperature) noexcept
{
    m_repository.temperature = temperature;
}

void MeteoMessageRepository::SetWet(float wet) noexcept
{
    m_repository.wet = wet;
}

void MeteoMessageRepository::SetNoConnectionState() noexcept
{
    ResetRepository();
}

void MeteoMessageRepository::SetCheckSumIsNotMatch() noexcept
{
    ResetRepository();
    m_repository.state = 1;
}

void MeteoMessageRepository::SetDeclineState() noexcept
{
    ResetRepository();
    m_repository.state = 2;
}

void MeteoMessageRepository::SetWrongData() noexcept
{
    ResetRepository();
    m_repository.state = 3;
}

void MeteoMessageRepository::SetGoodState() noexcept
{
    m_repository.state = 4;
}

void MeteoMessageRepository::SetTimeoutState() noexcept
{
    ResetRepository();
    m_repository.state = 5;
}

const DevicesMeteoKitGetMessage &MeteoMessageRepository::GetMessage() noexcept
{
    const QDateTime currentDateTime(QDateTime::currentDateTime());
    m_repository.sTimeMeasurement.usecs = currentDateTime.toMSecsSinceEpoch();
    m_repository.sTimeMeasurement.secs = currentDateTime.toSecsSinceEpoch();
    m_lastMessageDateTime = currentDateTime;
    return m_repository;
}

void MeteoMessageRepository::ResetRepository() noexcept
{
    m_repository.state = 0;
    m_repository.pressure = 0;
    m_repository.wet = 0;
    m_repository.temperature = 0;
}

std::string MeteoMessageRepository::GetLastMessageTime() const noexcept
{
    return m_lastMessageDateTime.toString(QLatin1String("hh:mm:ss")).toStdString();
}
