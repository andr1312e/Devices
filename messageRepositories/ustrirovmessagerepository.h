#ifndef MESSAGEREPOSITORIES_USTRIROVMESSAGEREPOSITORY_H
#define MESSAGEREPOSITORIES_USTRIROVMESSAGEREPOSITORY_H
#include <QDateTime>

#include "datamessage.h"

class UstrirovMessageRepository
{
public:
    explicit UstrirovMessageRepository();
    ~UstrirovMessageRepository();
public:
    void SetFvco(quint32 fvco);
    void SetDopler(int doplerFrequency);
    void SetDistance(quint32 distance);
    void SetDistanceToLocator(quint32 distanceToLocator);
    void SetGainTx(float gainTx);
    void SetGainRx(float gainRx);
    void SetAttenuator(quint8 attenuator);
    void SetWorkMode(quint8 workMode);

    void SetNoConnectionState();
    void SetTimeOutState();

    const DevicesAdjustingKitMessage &GetMessage();
public:
    quint16 GetDistanceToLocator() const;
    quint32 GetFvco() const;
private:
    void ResetRepository();

private:
    DevicesAdjustingKitMessage  m_repository;
};

#endif // MESSAGEREPOSITORIES_USTRIROVMESSAGEREPOSITORY_H
