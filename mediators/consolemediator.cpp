#include "consolemediator.h"

#include <QVector>
#include <iterator>
#include <QProcess>

#if _WIN32
     #include <Windows.h>
#endif

ConsoleMediator::ConsoleMediator(const std::string &logFilePath, RarmSocket *rarmSocket, MoxaMediator *moxa, UstirovMediator *ustirov, MeteoMediator *meteo, GeoSocket *geo, QObject *parent)
    : QObject(parent)
    , m_logFileMessagePath("Путь файлам логов: " + logFilePath)
    , m_rarmSocket(rarmSocket)
    , m_moxa(moxa)
    , m_ustirov(ustirov)
    , m_meteo(meteo)
    , m_geo(geo)
    , m_progress("Обновление раз в " + std::to_string(m_updateIterval) + "млс... ")
{
    setlocale(LC_ALL, "Russian"); //Функция setlocale() с аргументами
#if _WIN32
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
#endif
}

ConsoleMediator::~ConsoleMediator()
{

}

void ConsoleMediator::OnStartLog()
{
    startTimer(m_updateIterval, Qt::VeryCoarseTimer);
}

void ConsoleMediator::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)
    ClearConsole();
    ToPrintLogFilePath();
    PrintProcess();
    PrintRarmState();
    PrintMoxaState();
    PrintUkitState();
    PrintMeteoState();
    PrintGeoState();
}

void ConsoleMediator::ClearConsole()
{
#ifdef WINDOWS
    std::system("cls");
#else
    std::system ("clear");
#endif
}

void ConsoleMediator::ToPrintLogFilePath()
{
    std::cout << m_logFileMessagePath << std::endl;
}

void ConsoleMediator::PrintProcess()
{
    m_iteration++;
    if (20 == m_iteration)
    {
        m_iteration = 0;
        m_progress = "Обновление раз в " + std::to_string(m_updateIterval) + " млс... ";
    }
    else
    {
        m_progress += " " + std::to_string(m_iteration);
    }
    std::cout << m_progress << std::endl;
}

void ConsoleMediator::PrintRarmState()
{
    std::string message = "**RARM ";
    if (m_rarmSocket->IsRarmConnected())
    {
        message += "\033[32mV\033[0m "; //Зеленая галка
    }
    else
    {
        message += "\033[31mX\033[0m "; //Красный красный крест
        message += "\n    Ошибка: " + m_rarmSocket->GetRarmError();
    }
    message += "\n    Адрес рарма: " + m_rarmSocket->GetRarmAdress() + " Порт рарма: " + std::to_string(m_rarmSocket->GetRarmPort());
    std::cout << message << std::endl;
}

void ConsoleMediator::PrintMoxaState()
{
    std::string message = "**MOXA NPort-54501 ";
    if (m_moxa->IsMoxaConnected())
    {
        message += "\033[32mV\033[0m "; //Зеленая галка
    }
    else
    {
        message += "\033[31mX\033[0m "; //Красный красный крест
        message += "\n    Ошибка: Соединения нет";
    }
    message += "\n    Адрес мохи: " + m_moxa->GetMoxaIpAdress().toStdString();
    std::cout << message << std::endl;
}

void ConsoleMediator::PrintUkitState()
{
    std::string message = "**Юстировочный СЮИТ.687263.035 ";
    if (m_ustirov->IsUstirovConnected())
    {
        message += "\033[32mV\033[0m "; //Зеленая галка
        message += "\n    Порт юк: " + std::to_string(m_ustirov->GetUstirovPort());
        message += "\n    Сообщений в очереди: " + std::to_string(m_ustirov->GetMessagesCount());
        const QList<QByteArray> messagesList = m_ustirov->GetMessageList();
        for (const QByteArray &currentMessage : messagesList)
        {
            message += "\n          Сообщение: " + currentMessage.toHex().toStdString();
        }
        message += "\n    Данные:";
    }
    else
    {
        message += "\033[31mX\033[0m "; //Красный красный крест
        message += "\n    Ошибка: " + m_ustirov->GetLastUstirovErrorMessage().toStdString();
    }
    message += "\n    Порт юк: " + std::to_string(m_ustirov->GetUstirovPort());
    std::cout << message << std::endl;
}

void ConsoleMediator::PrintMeteoState()
{
    std::string message = "**Метеокомплект АМЯ2.702.090 Д1 ";
    if (m_meteo->IsMeteoConnected())
    {
        message += "\033[32mV\033[0m "; //Зеленая галка
    }
    else
    {
        message += "\033[31mX\033[0m "; //Красный красный крест
        message += "\n    Ошибка: " + m_meteo->GetMeteoError();
    }
    message += "\n    Данные:" + m_meteo->GetMeteoData();
    message += "\n    Время последнего сообщения: " + m_meteo->GetLastMessageTime();
    message += "\n    Порт метео: " + std::to_string(m_meteo->GetMeteoPort());
    std::cout << message << std::endl;

}

void ConsoleMediator::PrintGeoState()
{
    std::string message = "**Геодатчик u-blox ZED-F9P ";
    if (m_geo->IsGeoSocketConnected())
    {
        message += "\033[32mV\033[0m "; //Зеленая галка

    }
    else
    {
        message += "\033[31mX\033[0m "; //Красный красный крест
        message += "\n    Ошибка: " + m_geo->GetError();
    }
    message += "\n    Данные:" + m_geo->GetGeoData();
    std::cout << message << std::endl;
}
