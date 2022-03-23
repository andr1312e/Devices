#include "program.h"

Program::Program(int &argc, char **argv)
    : QCoreApplication(argc, argv)
    , m_sharedMemory(QStringLiteral("DEVICES"), this)
    , m_rarmAdress(QStringLiteral("127.0.0.1"))
    , m_rarmPort(4242)
{
    if(HasNoRunningInstances())
    {
        CreateObjects();
        ConnectObjects();
    }
}

Program::~Program()
{
    m_sharedMemory.detach();
    delete m_rarmSocket;
    delete m_meteoMediator;
    delete m_ustirovMediator;
    delete m_geoSocket;
}

bool Program::HasNoRunningInstances()
{
    if (m_sharedMemory.attach(QSharedMemory::ReadOnly))//есть участок с памятью с названием
    {
        m_sharedMemory.detach();//отключаемся
        qDebug()<< QStringLiteral("Program - Kill another instance of app from task manages Ctrl+Alt+Del+End Process Id: ")<< applicationPid();
        return false;//что то работает
    }
    else
    {
        if (m_sharedMemory.create(1, QSharedMemory::ReadWrite))//создаем участок с памятью
        {
            qDebug()<< QStringLiteral("Program - Devices Runned. Process id: ")<< applicationPid();
            return true;
        }
        else
        {
            qDebug() << QStringLiteral("error ") << m_sharedMemory.errorString();
            return false;
        }
    }
}

void Program::CreateObjects()
{
    m_rarmSocket=new RarmSocket(m_rarmAdress, m_rarmPort, this);
    m_moxaMediator=new MoxaMediator(QStringLiteral("moxaSettings.ini"), this);
    m_meteoMediator=new MeteoMediator(QStringLiteral("meteoSettings.ini"), this);
    m_ustirovMediator=new UstirovMediator(m_moxaMediator->GetMoxaIpAdress(), QStringLiteral("ustirovSettings.ini"), this);
    m_geoSocket=new GeoSocket(this);
}

void Program::ConnectObjects()
{
    connect(m_moxaMediator, &MoxaMediator::ToSendRarmMoxaWorksState, m_rarmSocket,  &RarmSocket::OnSendRarmMoxaWorksState);
    connect(m_meteoMediator, &MeteoMediator::ToSendRarmMeteoState, m_rarmSocket, &RarmSocket::OnSendRarmMeteoState);
    connect(m_ustirovMediator, &UstirovMediator::ToSendRarmUstirovState, m_rarmSocket, &RarmSocket::OnSendRarmUPCBState);
    connect(m_geoSocket, &GeoSocket::ToSendRarmGeoState, m_rarmSocket, &RarmSocket::OnSendRarmGeoMessage);
    connect(m_rarmSocket, &RarmSocket::ToSetUstirovState, m_ustirovMediator, &UstirovMediator::OnSetDataToUstirov);
    connect(m_rarmSocket, &RarmSocket::ToGetUstirovState, m_ustirovMediator, &UstirovMediator::OnGetDataFromUstirov);
}
