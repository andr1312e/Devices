#ifndef MESSAGEREPOSITORIES_USTRIROVMESSAGEREPOSITORY_H
#define MESSAGEREPOSITORIES_USTRIROVMESSAGEREPOSITORY_H
#include "datamessage.h"

class UstrirovMessageRepository
{
public:
    UstrirovMessageRepository();
    ~UstrirovMessageRepository();
    void SetFvco(quint32 Fvco);
    void SetDopler(int doplerFrequency);
    void SetGainTx(quint8 GainTx);
    void SetGainRx(quint8 GainRx);
    void SetAttenuator(quint8 attenuator);
    void SetWorkMode(quint8 workMode);
    void SetPhaseIncrement(quint16 phaseIncrement);
    void SetBadState();
    const DevicesAdjustingKitMessage &GetMessage() const;
    void ResetRepository();
private:
    DevicesAdjustingKitMessage  m_repository;
};

#endif // MESSAGEREPOSITORIES_USTRIROVMESSAGEREPOSITORY_H
