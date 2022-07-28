#include "geomessagerepository.h"

GeoMessageRepository::GeoMessageRepository()
{

}

GeoMessageRepository::~GeoMessageRepository()
{

}

void GeoMessageRepository::SetLatitude(float latitude) noexcept
{
    m_repository.latitude = latitude;
}

void GeoMessageRepository::SetLongitutude(float longtitude) noexcept
{
    m_repository.longtitude = longtitude;
}

void GeoMessageRepository::SetCompleteState() noexcept
{
    m_repository.state = 1;
}

void GeoMessageRepository::SetNoConnectionState() noexcept
{
    m_repository.state = 0;
}

void GeoMessageRepository::SetTimeOutState() noexcept
{
    m_repository.state = 2;
}

void GeoMessageRepository::SetNoAntennaState() noexcept
{
    m_repository.state = 3;
}

void GeoMessageRepository::SetWrongMessageState() noexcept
{
    m_repository.state = 4;
}

const DevicesGeoStateMessage &GeoMessageRepository::GetMessage() noexcept
{
    const QDateTime currentDateTime(QDateTime::currentDateTime());
    m_repository.sTimeMeasurement.usecs = currentDateTime.toMSecsSinceEpoch();
    m_repository.sTimeMeasurement.secs = currentDateTime.toSecsSinceEpoch();
    return m_repository;
}

void GeoMessageRepository::ResetMessage() noexcept
{
    m_repository.latitude = 0.0f;
    m_repository.longtitude = 0.0f;
    m_repository.height = 0.0f;
    m_repository.state = 1;
}
