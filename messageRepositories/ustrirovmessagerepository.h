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
    void SetNormalFvcoRx(quint32 fvcoRx) noexcept;
    void SetNormalFvcoTx(quint32 fvcoTx) noexcept;
    void SetNormalDopler(int doplerFrequency) noexcept;
    void SetNormalDistance(quint32 distance) noexcept;
    void SetDistanceToLocator(quint32 distanceToLocator) noexcept;
    void SetNormalGainTx(float gainTx) noexcept;
    void SetNormalGainRx(float gainRx) noexcept;
    void SetNormalAttenuator(quint8 attenuator) noexcept;
    void SetNormalWorkMode(quint8 workMode) noexcept;
    void SetNormalCompleteState() noexcept;

    void SetBpar(quint8 foId, bool isLcm, quint8 tksIndex, bool hasThreshold, quint16 threshold, int answerDelay) noexcept;

    void SetNoConnectionStateNormal() noexcept;
    void SetTimeOutStateNormal() noexcept;

    void SetNoConnectionStateBpar() noexcept;
    void SetTimeOutStateBpar() noexcept;

    const DevicesAdjustingKitMessage &GetNormalMessage();
    const DevicesBparAdjustingKitMessage &GetBparMessage();
public:
    quint16 GetDistanceToLocator() const noexcept;
    quint32 GetFvco() const noexcept;
private:
    void ResetNormalMessage() noexcept;
    void ResetBparMessage() noexcept;
private:
    DevicesAdjustingKitMessage     m_normalMessage;
    DevicesBparAdjustingKitMessage m_bparMessage;
};

#endif // MESSAGEREPOSITORIES_USTRIROVMESSAGEREPOSITORY_H
