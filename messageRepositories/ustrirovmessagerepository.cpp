#include "messageRepositories/ustrirovmessagerepository.h"

UstrirovMessageRepository::UstrirovMessageRepository()
{
    ResetRepository();
}

UstrirovMessageRepository::~UstrirovMessageRepository()
{

}

void UstrirovMessageRepository::SetFvco(quint32 Fvco)
{
    m_repository.Fvco=Fvco;
}

void UstrirovMessageRepository::SetDopler(int doplerFrequency)
{
    m_repository.DoplerFrequency=doplerFrequency;
}

void UstrirovMessageRepository::SetGainTx(quint8 GainTx)
{
    m_repository.GAIN_TX=GainTx;
}

void UstrirovMessageRepository::SetGainRx(quint8 GainRx)
{
    m_repository.GAIN_RX=GainRx;
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

void UstrirovMessageRepository::SetBadState()
{
    m_repository.WorkMode=255;
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
    m_repository.WorkMode=255;
}
