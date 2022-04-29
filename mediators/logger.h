#ifndef LOGGER_H
#define LOGGER_H
#include <fstream>
#include <cstdio>

class Logger
{
public:
    Logger(const std::string &logFileName)
        : m_logFileName(logFileName)
    {
        ClearLogFile();
    }

    ~Logger()
    {

    }
public:
    const std::string & GetLogFileName() const
    {
        return m_logFileName;
    }

    void Appends(const std::string &message) const
    {
        std::ofstream fout;
        fout.open(m_logFileName, std::ios::app);
        fout << message << std::endl;
        fout.flush();
        fout.close();
    }

private:
    void ClearLogFile() const
    {
        std::remove(m_logFileName.c_str());
    }


private:
    const std::string m_logFileName;
};



#endif // LOGGER_H
