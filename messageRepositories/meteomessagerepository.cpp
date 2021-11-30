#include "messageRepositories/meteomessagerepository.h"

MeteoMessageRepository::MeteoMessageRepository()
{
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
    m_repository.state=1;
}

void MeteoMessageRepository::SetDeclineState()
{
    ResetRepository();
    m_repository.state=2;
}

void MeteoMessageRepository::SetWrongData()
{
    ResetRepository();
    m_repository.state=3;
}

void MeteoMessageRepository::SetGoodState()
{
    m_repository.state=4;
}

void MeteoMessageRepository::SetTimeoutState()
{
   ResetRepository();
   m_repository.state=5;
}

const VOIStateMeteoMessage &MeteoMessageRepository::GetMessage() const
{
    return m_repository;
}

void MeteoMessageRepository::ResetRepository()
{
    m_repository.sTimeMeasurement.secs=0;
    m_repository.sTimeMeasurement.usecs=0;
    m_repository.state=0;
    m_repository.pressure=0;
    m_repository.wet=0;
    m_repository.temperature=0;
}
