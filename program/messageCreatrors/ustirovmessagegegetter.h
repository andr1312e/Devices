#ifndef MESSAGECREATORS_STATEMESSAGEGETTER_H
#define MESSAGECREATORS_STATEMESSAGEGETTER_H

#include <QByteArray>

#include <QByteArray>
#include <QDataStream>
#include <QtEndian>
#include <QDebug>
#include <QtMath>
#include <QObject>
#include "constants.h"

#include "../messageRepositories/ustrirovmessagerepository.h"
#include "../messageRepositories/geomessagerepository.h"

class UstirovMessageGetter: public QObject
{
    Q_OBJECT
public:
    explicit UstirovMessageGetter(const double f, const double fref,  UstrirovMessageRepository &messageRepository, GeoMessageRepository &geoRepository, QObject *parent);
    ~UstirovMessageGetter();
Q_SIGNALS:
    void ToAllNormalDataCollected();
    void ToAllBparDataCollected();
    void ToAllGeoCollected();
public:
    bool FillDataIntoStructFromMessage(const QByteArray &message);
    void SetNoConnectionStateNormal();
    void SetTimeOutState();
    const DevicesAdjustingKitMessage &GetMessageNormal();
    const DevicesBparAdjustingKitMessage &GetMessageBpar();
private:
    bool SaveFvcoRxToRepository(const QByteArray &message);
    bool SaveFvcoTxToRepository(const QByteArray &message);
    bool SaveDistanceToRepository(const QByteArray &message);
    bool SaveGainsToRepository(const QByteArray &message);
    bool SaveAttenuatorToRepository(const QByteArray &message);
    bool SaveWorkModeToRepository(const QByteArray &message);
    bool SaveDoplerToRepository(const QByteArray &message);
public:
    void SaveCoordinatesToRepository(const QByteArray &message);
private:
    bool SaveBparToRepository(const QByteArray &message);
    quint8 GetMask(quint8 pos, quint8 size ) const noexcept;
    quint16 GetIntFromMessage(const QByteArray &message) const;
    quint32 GetFractFromMessage(const QByteArray &message) const;
    bool GetDivFromMessage(const QByteArray &message) const;
    quint32 GetDistanceFromMessage(const QByteArray &message) const;
    quint32 ParceDelay(bool isLcm, quint16 delay) const;
    void ParceGeoMessage();
    double ParceLatitdeValue(const QByteArray &latitudeByteArray, int directionValue) const;
    double ParceLongtitude(const QByteArray &longtitudeByteArray, int directionValue) const;
private:
    const quint8 m_indexInByteArrayOfGettingMessageId;
    const double m_f;
    const double m_c = 299792458.0;
    const double m_Fref;
    const quint8 m_countOfWorkModes;
    UstrirovMessageRepository &m_messageRepository;
private:
    bool m_hasGeoHeader = false;
    QByteArray m_collectedGeoData;
    GeoMessageRepository &m_geoMessageRepository;
};

#endif // MESSAGECREATORS_STATEMESSAGEGETTER_H
