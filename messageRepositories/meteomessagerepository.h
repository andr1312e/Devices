#ifndef MESSAGEREPOSITORIES_METEOMESSAGEREPOSITORY_H
#define MESSAGEREPOSITORIES_METEOMESSAGEREPOSITORY_H
#include <QDateTime>

#include "datamessage.h"

class MeteoMessageRepository
{
public:
    explicit MeteoMessageRepository();
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
    const DevicesMeteoKitGetMessage &GetMessage();
    void ResetRepository();
public:
    std::string GetLastMessageTime() const;
private:
    DevicesMeteoKitGetMessage m_repository;
    QDateTime m_lastMessageDateTime;
};

#endif // MESSAGEREPOSITORIES_METEOMESSAGEREPOSITORY_H
