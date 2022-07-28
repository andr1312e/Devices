#ifndef MESSAGEREPOSITORIES_METEOMESSAGEREPOSITORY_H
#define MESSAGEREPOSITORIES_METEOMESSAGEREPOSITORY_H
#include <QDateTime>

#include "../datamessage.h"

class MeteoMessageRepository
{
public:
    explicit MeteoMessageRepository();
    ~MeteoMessageRepository();
public:
    void SetPressure(float pressure) noexcept;
    void SetTemperature(float temperature) noexcept;
    void SetWet(float wet) noexcept;

    void SetNoConnectionState() noexcept;
    void SetCheckSumIsNotMatch() noexcept;
    void SetDeclineState() noexcept;
    void SetWrongData() noexcept;
    void SetGoodState() noexcept;
    void SetTimeoutState() noexcept;
    const DevicesMeteoKitGetMessage &GetMessage() noexcept;
    void ResetRepository() noexcept;
public:
    std::string GetLastMessageTime() const noexcept;
private:
    DevicesMeteoKitGetMessage m_repository;
    QDateTime m_lastMessageDateTime;
};

#endif // MESSAGEREPOSITORIES_METEOMESSAGEREPOSITORY_H
