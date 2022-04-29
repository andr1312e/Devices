
#ifndef SINGLEINSTANCE_H
#define SINGLEINSTANCE_H

#include <QSharedMemory>
#include <QSystemSemaphore>
#include <QCryptographicHash>

class SingleInstanceMaker
{
public:
    explicit SingleInstanceMaker(const QString& sharedMemoryKey);
    ~SingleInstanceMaker();
public:
    bool CanRunApp();

private:
    bool IsAnotherRunning();
    void Release();
    QString GenerateKeyHash( const QString& key, const QString& salt ) const;

private:
    const QString m_strKey;
    const QString m_strMemLockKey;
    const QString m_strSharedMemKey;

    QSharedMemory    m_SharedMem;
    QSystemSemaphore m_lockSysSemph;

    Q_DISABLE_COPY( SingleInstanceMaker )
};

#endif // SINGLEINSTANCE_H
