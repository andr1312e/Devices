#include "ustrirovmessagerepository.h"



UstrirovMessageRepository::UstrirovMessageRepository()
{
    ResetNormalMessage();
}

UstrirovMessageRepository::~UstrirovMessageRepository()
{

}

void UstrirovMessageRepository::SetNormalFvcoRx(quint32 fvcoRx) noexcept
{
    m_normalMessage.FvcoRx = fvcoRx;
}

void UstrirovMessageRepository::SetNormalFvcoTx(quint32 fvcoTx) noexcept
{
    m_normalMessage.FvcoTx = fvcoTx;
}

void UstrirovMessageRepository::SetNormalDopler(int doplerFrequency) noexcept
{
    m_normalMessage.DoplerFrequency = doplerFrequency;
}

void UstrirovMessageRepository::SetNormalDistance(quint32 distance) noexcept
{
    m_normalMessage.Distance = distance;
}

void UstrirovMessageRepository::SetDistanceToLocator(quint32 distanceToLocator) noexcept
{
    m_normalMessage.DistanceToLocator = distanceToLocator;
    m_bparMessage.DistanceToLocator = distanceToLocator;
}

void UstrirovMessageRepository::SetNormalGainTx(float gainTx) noexcept
{
    m_normalMessage.GAIN_TX = gainTx;
}

void UstrirovMessageRepository::SetNormalGainRx(float gainRx) noexcept
{
    m_normalMessage.GAIN_RX = gainRx;
}

void UstrirovMessageRepository::SetNormalAttenuator(quint8 attenuator) noexcept
{
    m_normalMessage.Attenuator = attenuator;
}

void UstrirovMessageRepository::SetNormalWorkMode(quint8 workMode) noexcept
{
    m_normalMessage.WorkMode = workMode;
}

void UstrirovMessageRepository::SetNormalCompleteState() noexcept
{
    m_normalMessage.state = 1;
}

void UstrirovMessageRepository::SetBpar(quint8 foId, bool isLcm, quint8 tksIndex, int answerDelay) noexcept
{
    m_bparMessage.foId = foId;
    m_bparMessage.isLcm = isLcm;
    m_bparMessage.tksIndex = tksIndex;
    m_bparMessage.answerDelay = answerDelay;
    m_bparMessage.WorkMode = 2;
    m_bparMessage.state = 1;
}

void UstrirovMessageRepository::SetThreshold(bool hasThreshold, quint16 threshold) noexcept
{
    m_bparMessage.hasThreshold = hasThreshold;
    m_bparMessage.threshold = threshold;
}

void UstrirovMessageRepository::SetNoConnectionStateNormal() noexcept
{
    ResetNormalMessage();
    m_normalMessage.state = 0;
}

void UstrirovMessageRepository::SetTimeOutStateNormal() noexcept
{
    ResetNormalMessage();
    m_normalMessage.state = 2;
}

void UstrirovMessageRepository::SetNoConnectionStateBpar() noexcept
{
    ResetBparMessage();
    m_bparMessage.state = 0;
}

void UstrirovMessageRepository::SetTimeOutStateBpar() noexcept
{
    ResetBparMessage();
    m_bparMessage.state = 2;
}

const DevicesAdjustingKitMessage &UstrirovMessageRepository::GetNormalMessage() noexcept
{
    const QDateTime currentDateTime(QDateTime::currentDateTime());
    m_normalMessage.sTimeMeasurement.usecs = currentDateTime.toMSecsSinceEpoch();
    m_normalMessage.sTimeMeasurement.secs = currentDateTime.toSecsSinceEpoch();
    return m_normalMessage;
}

const DevicesBparAdjustingKitMessage &UstrirovMessageRepository::GetBparMessage() noexcept
{
    const QDateTime currentDateTime(QDateTime::currentDateTime());
    m_bparMessage.sTimeMeasurement.usecs = currentDateTime.toMSecsSinceEpoch();
    m_bparMessage.sTimeMeasurement.secs = currentDateTime.toSecsSinceEpoch();
    return m_bparMessage;
}

void UstrirovMessageRepository::ResetNormalMessage() noexcept
{
    m_normalMessage.Attenuator = 0;
    m_normalMessage.Distance = 0;
    m_normalMessage.DoplerFrequency = 0;
    m_normalMessage.FvcoRx = 0;
    m_normalMessage.FvcoTx = 0;
    m_normalMessage.GAIN_RX = 0;
    m_normalMessage.GAIN_TX = 0;
    m_normalMessage.WorkMode = 0;
    m_normalMessage.state = 1;
}

void UstrirovMessageRepository::ResetBparMessage() noexcept
{
    m_bparMessage.answerDelay = 0;
    m_bparMessage.foId = 0;
    m_bparMessage.hasThreshold = false;
    m_bparMessage.isLcm = false;
    m_bparMessage.threshold = 0;
    m_bparMessage.tksIndex = 0;
    m_bparMessage.WorkMode = 0;
    m_bparMessage.state = 1;
}

quint16 UstrirovMessageRepository::GetDistanceToLocator() const noexcept
{
    return m_normalMessage.DistanceToLocator;
}

quint32 UstrirovMessageRepository::GetFvco() const noexcept
{
    return m_normalMessage.FvcoRx;
}
