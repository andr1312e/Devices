#include "devices.h"

Devices::Devices()
    : QObject(Q_NULLPTR)
//    , m_rarmAdress(QLatin1String("192.168.115.195"))
    , m_rarmAdress(QLatin1String("127.0.0.1"))
    , m_rarmPort(4242)
{
    CreateObjects();
    PrepareObjects();
    ConnectObjects();
    StartObjects();
}

Devices::~Devices()
{
    delete m_logger;
    delete m_rarmSocket;
    delete m_moxaMediator;
    delete m_meteoMediator;
    delete m_ustirovMediator;
    delete m_geoSocket;
    delete m_consoleMediator;
    delete m_consoleThread;
}

void Devices::CreateObjects()
{
#ifdef QT_DEBUG
    m_logger = new Logger(QDir::homePath().toStdString() + "/devicesDebug.log");
#else
    m_logger = new Logger(QDir::homePath().toStdString() + "/devices.log");
#endif
    m_rarmSocket = new RarmSocket(m_logger, m_rarmAdress, m_rarmPort, this);
    m_moxaMediator = new MoxaMediator(this);
    m_meteoMediator = new MeteoMediator(m_logger, this);
    m_ustirovMediator = new UstirovMediator(m_logger, m_moxaMediator->GetMoxaIpAdress(), QStringLiteral("ustirovSettings.ini"), this);
    m_geoSocket = new GeoSocket(this);
    m_consoleMediator = new ConsoleMediator(m_logger->GetLogFileName(), m_rarmSocket, m_moxaMediator, m_ustirovMediator, m_meteoMediator, m_geoSocket, Q_NULLPTR);
    m_consoleThread = new QThread(Q_NULLPTR);
    ProFile::ApplyEnviroment();
}

void Devices::PrepareObjects()
{
    m_consoleMediator->moveToThread(m_consoleThread);
}

void Devices::ConnectObjects()
{
    connect(m_moxaMediator, &MoxaMediator::ToSendRarmMoxaWorksState, m_rarmSocket,  &RarmSocket::OnSendRarmMoxaWorksState);
    connect(m_meteoMediator, &MeteoMediator::ToSendRarmMeteoState, m_rarmSocket, &RarmSocket::OnSendRarmMeteoState);
    connect(m_ustirovMediator, &UstirovMediator::ToSendRarmUstirovState, m_rarmSocket, &RarmSocket::OnSendRarmUPCBState);
    connect(m_ustirovMediator, &UstirovMediator::ToSendPcbWork, m_moxaMediator, &MoxaMediator::OnRegisterPCBState);
    connect(m_geoSocket, &GeoSocket::ToSendRarmGeoState, m_rarmSocket, &RarmSocket::OnSendRarmGeoMessage);
    connect(m_rarmSocket, &RarmSocket::ToSetUstirovState, m_ustirovMediator, &UstirovMediator::OnSetDataToUstirov);
    connect(m_rarmSocket, &RarmSocket::ToGetUstirovState, m_ustirovMediator, &UstirovMediator::OnGetDataFromUstirov);
    connect(m_consoleThread, &QThread::started, m_consoleMediator, &ConsoleMediator::OnStartLog);
}

void Devices::StartObjects()
{
    m_consoleThread->start();
}
