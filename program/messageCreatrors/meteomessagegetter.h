#ifndef METEOMESSAGEGETTER_H
#define METEOMESSAGEGETTER_H

#include <QObject>
#include <QSharedPointer>

#include "../messageRepositories/meteomessagerepository.h"

enum class METEO
{
    METEO_PRESSURE      = 1, // давление
    METEO_TEMPERATURE   = 2, // температура
    METEO_WET           = 3  // влажность
};

class MeteoMessageGetter : public QObject
{
    Q_OBJECT
public:
    explicit MeteoMessageGetter(QObject *parent);
    ~MeteoMessageGetter();
Q_SIGNALS:
    void ToAllDataCollected();
public:
    void ParseMessage(const QByteArray &message);
    const DevicesMeteoKitGetMessage &MessageTimeOut() noexcept;
    const DevicesMeteoKitGetMessage &NoConnectionMessage() noexcept;
    const DevicesMeteoKitGetMessage &GetMessage() noexcept;
    std::string GetLastMessageTime() const noexcept;
private:
    MeteoMessageRepository *const m_repository;


};

#endif // METEOMESSAGEGETTER_H
