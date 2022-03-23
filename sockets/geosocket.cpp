#include "geosocket.h"

GeoSocket::GeoSocket(QObject *parent)
    : QObject(parent)
    , m_dataNotFull(false)
    , m_hasAnswer(false)
{
    CreateObjects();
    InitObjects();
    ConnectObjects();
}

GeoSocket::~GeoSocket()
{
    delete m_connectionPort;
}

void GeoSocket::CreateObjects()
{
    m_connectionPort=new QSerialPort(this);
}

void GeoSocket::InitObjects()
{
    m_connectionPort->setBaudRate(QSerialPort::Baud115200);
    m_connectionPort->setDataBits(QSerialPort::Data8);
    m_connectionPort->setParity(QSerialPort::EvenParity);
    m_connectionPort->setStopBits(QSerialPort::OneStop);
    const QList<QSerialPortInfo> serialPortInfoList(QSerialPortInfo::availablePorts());
    for (const QSerialPortInfo &serialPortInfo: serialPortInfoList)
    {
        qDebug() << serialPortInfo.portName();
    }
    m_connectionPort->setPortName(serialPortInfoList.back().portName());
    startTimer(4000, Qt::VeryCoarseTimer);
}

void GeoSocket::ConnectObjects()
{
//    connect(m_connectionPort, &QSerialPort::errorOccurred, this,  &GeoSocket::OnClose);
    connect(m_connectionPort, &QSerialPort::readyRead, this, &GeoSocket::OnReadyRead);
}

void GeoSocket::OnReadyRead()
{
    const QByteArray data(m_connectionPort->readAll());
    const int indexOfHeader=data.indexOf("$GNGGA");
    if(indexOfHeader!=-1)
    {
        const int indexOfAsterisk=data.indexOf('*', indexOfHeader);
        if(-1==indexOfAsterisk)
        {
            m_collectedData=data.mid(indexOfHeader);
            m_dataNotFull=true;

        }
        else
        {
            m_collectedData=data.mid(indexOfHeader, indexOfAsterisk-indexOfHeader+1);
            m_dataNotFull=false;
            ParseMessage();
        }
    }
    else
    {

        if(m_dataNotFull)
        {
            int indexOfAsterisk=data.indexOf('*', indexOfHeader);
            if(indexOfAsterisk!=-1)
            {
                m_collectedData.append(data.left(indexOfHeader+1));
                ParseMessage();
            }
            else
            {
                m_collectedData.clear();
            }
        }
    }
}

void GeoSocket::OnClose()
{
    if(m_connectionPort->isOpen())
        m_connectionPort->close();
}

void GeoSocket::timerEvent(QTimerEvent *event)
{
    if (m_connectionPort->isOpen())
    {
        if(m_geoState.state)
        {
            qDebug()<< "GS : Reset state";
            m_geoState.state=false;
        }
        else
        {
            qDebug()<< "GS : No data";
            Q_EMIT ToSendRarmGeoState(m_geoState);
        }
    }
    else
    {
        if(m_connectionPort->open(QIODevice::ReadOnly))
        {
            qDebug()<< "GS : Opened socket";
            m_geoState.state=false;
        }
        else
        {
            qDebug()<< "GS : Can't open socket "<< m_connectionPort->errorString();
            ResetState();
            Q_EMIT ToSendRarmGeoState(m_geoState);
        }
    }

}

void GeoSocket::ParseMessage()
{
    if(m_collectedData.startsWith("$GNGGA") && m_collectedData.endsWith('*') && m_collectedData.count()==70)
    {
        qDebug()<< m_collectedData;
        m_geoState.latitude=ParceLatitde();
        m_geoState.longtitude=ParceLongtitude();
        m_geoState.height=ParceHeight();
        m_geoState.state=true;
        qDebug()<< "GS : Send message";
        Q_EMIT ToSendRarmGeoState(m_geoState);
    }
}

float GeoSocket::ParceLatitde()
{
    //"ГГММ.МММММ". Например  "5546.95900" = 55°46.959' = 55.78265°.
    //ГГ - градусы; ММ.ММММ - минуты и десятитысячные доли минут.
    //Для ГГММ.ММММ → ГГ.ГГГГГГ перевода необходимо использовать формулу ГГ + (ММ.ММММ ÷ 60):
    const char gradusesFirst=m_collectedData.at(17);
    const char gradusesSecond=m_collectedData.at(18);
    const int graduses=(gradusesFirst-48)*10+(gradusesSecond-48);
    const char minutesDecimalFirst=m_collectedData.at(19);
    const char minutesDecimalSecond=m_collectedData.at(20);
    const char minutesFractFirst=m_collectedData.at(22);
    const char minutesFractSecond=m_collectedData.at(23);
    const char minutesFractThird=m_collectedData.at(24);
    const char minutesFractFourth=m_collectedData.at(25);
    const char minutesFractFive=m_collectedData.at(26);
    const float minutes=(minutesDecimalFirst-48.0)*10.0f+(minutesDecimalSecond-48.0)
                          +(minutesFractFirst-48.0)/10.0f+(minutesFractSecond-48.0)/100.0f
                          +(minutesFractThird-48.0)/1000.0f+(minutesFractFourth-48.0)/10000.0f+
                          (minutesFractFive-48.0)/100000.0f;
    //    qDebug()<< "ParceLatitde " << graduses << "   " << minutes;
    const char latDirection=m_collectedData.at(28);
    const int directionValue= latDirection=='N' ? 1 : -1;
    const float realCoordinate=directionValue*graduses+minutes/60.0f;
    return realCoordinate;

}

float GeoSocket::ParceLongtitude()
{
    const char gradusesFirst=m_collectedData.at(30);
    const char gradusesSecond=m_collectedData.at(31);
    const char gradusesThird=m_collectedData.at(32);
    const int graduses=(gradusesFirst-48)*100+(gradusesSecond-48)*10+(gradusesThird-48);
    const char minutesDecimalFirst=m_collectedData.at(33);
    const char minutesDecimalSecond=m_collectedData.at(34);
    const char minutesFractFirst=m_collectedData.at(36);
    const char minutesFractSecond=m_collectedData.at(37);
    const char minutesFractThird=m_collectedData.at(38);
    const char minutesFractFourth=m_collectedData.at(39);
    const char minutesFractFive=m_collectedData.at(40);
    const float minutes=(minutesDecimalFirst-48.0)*10.0f+(minutesDecimalSecond-48.0)
                          +(minutesFractFirst-48.0)/10.0f+(minutesFractSecond-48.0)/100.0f
                          +(minutesFractThird-48.0)/1000.0f+(minutesFractFourth-48.0)/10000.0f+
                          (minutesFractFive-48.0)/100000.0f;
    //    qDebug()<< "ParceLongtitude " << graduses << "   " << minutes;
    const char direction=m_collectedData.at(42);
    const int directionValue= direction=='E' ? 1 : -1;
    const float realCoordinate=directionValue*graduses+minutes/60.0f;
    return realCoordinate;
}

float GeoSocket::ParceHeight()
{
    QByteArray arr=m_collectedData.mid(54, 5);
    float height=arr.toDouble();
    const char typeOfHeight=m_collectedData.at(60);
    if(typeOfHeight=='M')
    {
        return height;
    }
    else
    {
        qFatal("Wrong height");
    }
}

void GeoSocket::ResetState()
{
    m_geoState.height=0.0f;
    m_geoState.latitude=0.0f;
    m_geoState.longtitude=0.0f;
    m_geoState.state=0;
}
