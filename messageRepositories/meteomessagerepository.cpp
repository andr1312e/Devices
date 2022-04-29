#include "messageRepositories/meteomessagerepository.h"

MeteoMessageRepository::MeteoMessageRepository()
{
    m_repository.sTimeMeasurement.usecs=0;
    m_repository.sTimeMeasurement.secs=0;
    ResetRepository();
}

MeteoMessageRepository::~MeteoMessageRepository()
{

}

void MeteoMessageRepository::SetPressure(float pressure)
{
    m_repository.pressure=pressure;
}

void MeteoMessageRepository::SetTemperature(float temperature)
{
    m_repository.temperature=temperature;
}

void MeteoMessageRepository::SetWet(float wet)
{
    m_repository.wet=wet;
}

void MeteoMessageRepository::SetNoConnectionState()
{
    ResetRepository();
}

void MeteoMessageRepository::SetCheckSumIsNotMatche()
{
    ResetRepository();
    const QDateTime currentDateTime(QDateTime::currentDateTime());
    m_repository.sTimeMeasurement.usecs=currentDateTime.toMSecsSinceEpoch();
    m_repository.sTimeMeasurement.secs=currentDateTime.toSecsSinceEpoch();
    m_lastMessageDateTime=currentDateTime;
    m_repository.state=1;
}

void MeteoMessageRepository::SetDeclineState()
{
    ResetRepository();
    const QDateTime currentDateTime(QDateTime::currentDateTime());
    m_repository.sTimeMeasurement.usecs=currentDateTime.toMSecsSinceEpoch();
    m_repository.sTimeMeasurement.secs=currentDateTime.toSecsSinceEpoch();
    m_lastMessageDateTime=currentDateTime;
    m_repository.state=2;
}

void MeteoMessageRepository::SetWrongData()
{
    ResetRepository();
    const QDateTime currentDateTime(QDateTime::currentDateTime());
    m_repository.sTimeMeasurement.usecs=currentDateTime.toMSecsSinceEpoch();
    m_repository.sTimeMeasurement.secs=currentDateTime.toSecsSinceEpoch();
    m_lastMessageDateTime=currentDateTime;
    m_repository.state=3;
}

void MeteoMessageRepository::SetGoodState()
{
    const QDateTime currentDateTime(QDateTime::currentDateTime());
    m_repository.sTimeMeasurement.usecs=currentDateTime.toMSecsSinceEpoch();
    m_repository.sTimeMeasurement.secs=currentDateTime.toSecsSinceEpoch();
    m_lastMessageDateTime=currentDateTime;
    m_repository.state=4;
}

void MeteoMessageRepository::SetTimeoutState()
{
    ResetRepository();
    const QDateTime currentDateTime(QDateTime::currentDateTime());
    m_repository.sTimeMeasurement.usecs=currentDateTime.toMSecsSinceEpoch();
    m_repository.sTimeMeasurement.secs=currentDateTime.toSecsSinceEpoch();
    m_lastMessageDateTime=currentDateTime;
    m_repository.state=5;
}

const DevicesMeteoKitGetMessage &MeteoMessageRepository::GetMessage()
{
    return m_repository;
}

void MeteoMessageRepository::ResetRepository()
{
    m_repository.state=0;
    m_repository.pressure=0;
    m_repository.wet=0;
    m_repository.temperature=0;
}

std::string MeteoMessageRepository::GetLastMessageTime() const
{
    return m_lastMessageDateTime.toString(QStringLiteral("hh:mm:ss")).toStdString();
}
