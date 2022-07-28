#ifndef MESSAGEREPOSITORIES_GEOMESSAGEREPOSITORY_H
#define MESSAGEREPOSITORIES_GEOMESSAGEREPOSITORY_H
#include "../datamessage.h"

#include <QDateTime>

class GeoMessageRepository
{
public:
    explicit GeoMessageRepository();
    ~GeoMessageRepository();
public:
    void SetLatitude(float latitude) noexcept;
    void SetLongitutude(float longtitude) noexcept;

    void SetCompleteState() noexcept;
    void SetNoConnectionState() noexcept;
    void SetTimeOutState() noexcept;
    void SetNoAntennaState() noexcept;
    void SetWrongMessageState() noexcept;
public:
    const DevicesGeoStateMessage &GetMessage() noexcept;
private:
    void ResetMessage() noexcept;
private:
    DevicesGeoStateMessage m_repository;
};

#endif // MESSAGEREPOSITORIES_GEOMESSAGEREPOSITORY_H
