#ifndef METEOMESSAGEGETTER_H
#define METEOMESSAGEGETTER_H

#include <QObject>
#include <QSharedPointer>
#include "messageRepositories/meteomessagerepository.h"

enum METEO
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
    DevicesMeteoKitGetMessage &MessageTimeOut();
    DevicesMeteoKitGetMessage &NoConnectionMessage();
    DevicesMeteoKitGetMessage &GetMessage() const;
private:
    QSharedPointer<MeteoMessageRepository> m_repository;

};

#endif // METEOMESSAGEGETTER_H
