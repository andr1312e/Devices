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
    void ToSendRarmGeoState(DevicesGeoStateMessage  &state);
private Q_SLOTS:
    void OnReadyRead();
    void OnClose();
protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
private:
    void ParseMessage();
    float ParceLatitde();
    float ParceLongtitude();
    float ParceHeight();
    void ResetState();
private:
    DevicesGeoStateMessage  m_geoState;
    QSerialPort *m_connectionPort;
    QByteArray m_collectedData;
    bool m_dataNotFull;
    bool m_hasAnswer;
};

#endif // GEOSOCKET_H
