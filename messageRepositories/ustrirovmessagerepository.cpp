#include "messageRepositories/ustrirovmessagerepository.h"

UstrirovMessageRepository::UstrirovMessageRepository()
{
    ResetRepository();
}

UstrirovMessageRepository::~UstrirovMessageRepository()
{

}

void UstrirovMessageRepository::SetFvco(quint32 fvco)
{
    m_repository.Fvco=fvco;
}

void UstrirovMessageRepository::SetDopler(int doplerFrequency)
{
    m_repository.DoplerFrequency=doplerFrequency;
}

void UstrirovMessageRepository::SetDistance(quint16 distance)
{
    m_repository.Distance=distance;
}

void UstrirovMessageRepository::SetDistanceToLocator(quint16 distanceToLocator)
{
    m_repository.DistanceToLocator=distanceToLocator;
}

void UstrirovMessageRepository::SetGainTx(float gainTx)
{
    m_repository.GAIN_TX=gainTx;
}

void UstrirovMessageRepository::SetGainRx(float gainRx)
{
    m_repository.GAIN_RX=gainRx;
}

void UstrirovMessageRepository::SetAttenuator(quint8 attenuator)
{
    m_repository.Attenuator=attenuator;
}

void UstrirovMessageRepository::SetWorkMode(quint8 workMode)
{
    m_repository.WorkMode=workMode;
}

void UstrirovMessageRepository::SetPhaseIncrement(quint16 phaseIncrement)
{
    m_repository.PhaseIncrement=phaseIncrement;
}

void UstrirovMessageRepository::SetNoConnectionState()
{
    ResetRepository();
    m_repository.state=0;
}

void UstrirovMessageRepository::SetTimeOutState()
{
    ResetRepository();
    m_repository.state=2;
}

DevicesAdjustingKitMessage &UstrirovMessageRepository::GetMessage()
{
    return m_repository;
}

void UstrirovMessageRepository::ResetRepository()
{
    m_repository.Attenuator=0;
    m_repository.Distance=0;
    m_repository.DoplerFrequency=0;
    m_repository.Fvco=0;
    m_repository.GAIN_RX=0;
    m_repository.GAIN_TX=0;
    m_repository.PhaseIncrement=0;
    m_repository.WorkMode=0;
    m_repository.state=1;
}

const quint16 UstrirovMessageRepository::GetDistanceToLocator() const
{
    return m_repository.DistanceToLocator;
}

const quint32 UstrirovMessageRepository::GetFvco() const
{
    return m_repository.Fvco;
}
