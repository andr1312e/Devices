#ifndef MESSAGEREPOSITORIES_METEOMESSAGEREPOSITORY_H
#define MESSAGEREPOSITORIES_METEOMESSAGEREPOSITORY_H
#include "datamessage.h"

class MeteoMessageRepository
{
public:
    MeteoMessageRepository();
    ~MeteoMessageRepository();
public:
    void SetPressure(float pressure);
    void SetTemperature(float temperature);
    void SetWet(float wet);

    void SetNoConnectionState();
    void SetCheckSumIsNotMatche();
    void SetDeclineState();
    void SetWrongData();
    void SetGoodState();
    void SetTimeoutState();
    VOIStateMeteoMessage &GetMessage();
    void ResetRepository();
private:
    VOIStateMeteoMessage m_repository;
};

#endif // MESSAGEREPOSITORIES_METEOMESSAGEREPOSITORY_H
