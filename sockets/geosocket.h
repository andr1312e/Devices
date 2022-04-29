#ifndef GEOSOCKET_H
#define GEOSOCKET_H

#include <QObject>
#include <QSerialPort>
#include <QDebug>
#include <QSerialPortInfo>
#include "datamessage.h"

class GeoSocket : public QObject
{
    Q_OBJECT
public:
    explicit GeoSocket(QObject *parent);
    ~GeoSocket();
private:
    void CreateObjects();
    void InitObjects();
    void ConnectObjects();
Q_SIGNALS:
    void ToSendRarmGeoState(const DevicesGeoStateMessage  &state);
private Q_SLOTS:
    void OnReadyRead();
    void OnClose();
public:
    bool IsGeoSocketConnected() const;
    std::string GetError() const;
    std::string GetGeoData() const;
protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    void ParseMessage();
    float ParceLatitde();
    float ParceLongtitude();
    float ParceHeight();
    void ResetState();
private:
    bool m_dataNotFull;
    bool m_hasAnswer;
private:
    DevicesGeoStateMessage  m_geoState;
    QSerialPort *m_connectionPort;
    QByteArray m_collectedData;
};

#endif // GEOSOCKET_H
