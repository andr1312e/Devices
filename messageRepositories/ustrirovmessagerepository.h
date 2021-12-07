#ifndef MESSAGEREPOSITORIES_USTRIROVMESSAGEREPOSITORY_H
#define MESSAGEREPOSITORIES_USTRIROVMESSAGEREPOSITORY_H
#include "datamessage.h"

class UstrirovMessageRepository
{
public:
    UstrirovMessageRepository();
    ~UstrirovMessageRepository();
    void SetFvco(quint32 fvco);
    void SetDopler(int doplerFrequency);
    void SetDistance(quint16 distance);
    void SetDistanceToLocator(quint16 distanceToLocator);
    void SetGainTx(quint8 gainTx);
    void SetGainRx(quint8 gainRx);
    void SetAttenuator(quint8 attenuator);
    void SetWorkMode(quint8 workMode);
    void SetPhaseIncrement(quint16 phaseIncrement);

    void SetNoConnectionState();
    void SetTimeOutState();

    DevicesAdjustingKitMessage &GetMessage();
public:
    const quint16 GetDistanceToLocator() const;
    const quint32 GetFvco() const;
private:
    void ResetRepository();

private:
    DevicesAdjustingKitMessage  m_repository;
};

#endif // MESSAGEREPOSITORIES_USTRIROVMESSAGEREPOSITORY_H
