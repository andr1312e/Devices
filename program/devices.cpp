#include "devices.h"

Devices::Devices()
    : QObject(Q_NULLPTR)
#ifdef Q_OS_WINDOWS
//    , m_rarmAdress(QLatin1String("192.168.115.195"))
    , m_rarmAdress(QLatin1String("192.168.115.240"))
#else
    , m_rarmAdress(QLatin1String("127.0.0.1"))
#endif
    , m_rarmPort(4242)
{
    CreateObjects();
    PrepareConsole();
    ConnectObjects();
    StartConsoleThread();
}

Devices::~Devices()
{
    delete m_logger;
    delete m_rarmSocket;
    delete m_moxaMediator;
    delete m_meteoMediator;
    delete m_ustirovMediator;
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
    m_ustirovMediator = new UstirovMediator(m_logger, m_moxaMediator->GetMoxaIpAdress(), this);
    m_consoleMediator = new ConsoleMediator(m_logger->GetLogFileName(), m_rarmSocket, m_moxaMediator, m_ustirovMediator, m_meteoMediator, Q_NULLPTR);
    m_consoleThread = new QThread(Q_NULLPTR);
    ProFile::ApplyTextToProfile();
}

void Devices::PrepareConsole()
{
    m_consoleMediator->moveToThread(m_consoleThread);
}

void Devices::ConnectObjects()
{
    connect(m_moxaMediator, &MoxaMediator::ToSendRarmMoxaWorksState, m_rarmSocket,  &RarmSocket::OnSendRarmMoxaWorksState);
    connect(m_meteoMediator, &MeteoMediator::ToSendRarmMeteoState, m_rarmSocket, &RarmSocket::OnSendRarmMeteoState);
    connect(m_ustirovMediator, &UstirovMediator::ToSendRarmNormalState, m_rarmSocket, &RarmSocket::OnSendRarmNormalState);
    connect(m_ustirovMediator, &UstirovMediator::ToSendRarmBparState, m_rarmSocket, &RarmSocket::OnSendRarmBparState);
    connect(m_ustirovMediator, &UstirovMediator::ToSendRarmGeoState, m_rarmSocket, &RarmSocket::OnSendRarmGeoState);
    connect(m_ustirovMediator, &UstirovMediator::ToSendPcbWork, m_moxaMediator, &MoxaMediator::OnSendPcbWork);

    connect(m_rarmSocket, &RarmSocket::ToSetUstirovNormalState, m_ustirovMediator, &UstirovMediator::OnSetUstirovNormalState);
    connect(m_rarmSocket, &RarmSocket::ToSetUstirovBparState, m_ustirovMediator, &UstirovMediator::OnSetUstirovBparState);
    connect(m_rarmSocket, &RarmSocket::ToRequestGeoState, m_ustirovMediator, &UstirovMediator::OnRequestGeoState);

    connect(m_consoleThread, &QThread::started, m_consoleMediator, &ConsoleMediator::OnStartLog);
}

void Devices::StartConsoleThread()
{
    m_consoleThread->start();
}
