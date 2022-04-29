#include "singleinstancemaker.h"

SingleInstanceMaker::SingleInstanceMaker(const QString &sharedMemoryKey)
    : m_strKey          ( sharedMemoryKey )                                     // ключ
    , m_strMemLockKey   ( GenerateKeyHash( sharedMemoryKey, "_memLockKey" ) )   // уникальный хэш по ключу
    , m_strSharedMemKey ( GenerateKeyHash( sharedMemoryKey, "_sharedmemKey" ) ) // другой хэш по ключу
    , m_SharedMem       ( m_strSharedMemKey )                       // создать разделяемую часть памяти на основе второго хэша
    , m_lockSysSemph    ( m_strMemLockKey, 1 )                      // создать общесистемный семафор со счетом «1» на основе первого хэша
{
    // Присоединение и отсоединение от блока общей памяти (в деструкторе), чтобы избежать ошибки Linux.
    // Это создает и уничтожает блок памяти, что-то вроде механизма «сброса».
    // Ссылка на исправление для linux: http://habrahabr.ru/post/173281/
    QSharedMemory fix( m_strSharedMemKey );
    fix.attach();
}

SingleInstanceMaker::~SingleInstanceMaker()
{
    // Удалить блок памяти, если он существует.
    Release();
}

bool SingleInstanceMaker::CanRunApp()
{
    // Проверяем, запущен ли другой, проверяя, существует ли уже блок общей памяти
    if (IsAnotherRunning())
    {
        return false;
    }

    // Блокировка всей системы
    m_lockSysSemph.acquire();
    // Создаем блок памяти, учитывая, что его раньше не было
    const bool result = m_SharedMem.create( sizeof( quint64 ) );
    // Разблокировать всю систему
    m_lockSysSemph.release();


    // Если блок создан успешно, возвращаем true
    if (result)
    {
        return true;
    }
    else
    {
       // Если не удалось создать ошибку возврата, возможно, запущен другой экземпляр
        Release();
        return false;
    }


}

bool SingleInstanceMaker::IsAnotherRunning()
{
    // QSharedMemory::isAttached возвращает true, если этот процесс присоединен к сегменту разделяемой памяти.
    // Итак, если ЭТОТ ПРОЦЕСС ПРИСОЕДИНЕН, ТО ДРУГОЙ ПРОЦЕСС НЕ ЗАПУСКАЕТСЯ
    if ( m_SharedMem.isAttached() )
    {
        return false;
    }

    // Блокировка всей системы
    m_lockSysSemph.acquire();

    // Попытка присоединения, в случае успеха это означает, что блок уже существует, поэтому выполняется другой
    // В случае неудачи это означает, что блок памяти еще не создан
    const bool isRunning = m_SharedMem.attach();

    if (isRunning)
    {
        // Отсоединение, если присоединение прошло успешно, блок памяти не нужен, если есть другой работающий
        m_SharedMem.detach();
    }

    // Разблокировать всю систему
    m_lockSysSemph.release();

    return isRunning;
}

void SingleInstanceMaker::Release()
{
    // Блокировка всей системы
    m_lockSysSemph.acquire();
    // Отсоединить, если он подключен (удалить блок общей памяти, если он существует)
    if (m_SharedMem.isAttached())
    {
        m_SharedMem.detach();
    }
    // Разблокировать всю систему
    m_lockSysSemph.release();
}

QString SingleInstanceMaker::GenerateKeyHash( const QString& key, const QString& salt ) const
{
    QByteArray data;

    data.append( key.toUtf8() );
    data.append( salt.toUtf8() );
    data = QCryptographicHash::hash( data, QCryptographicHash::Sha1 ).toHex();

    return data;
}
