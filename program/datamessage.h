#ifndef DATAMESSAGE_H
#define DATAMESSAGE_H

#include <QtGlobal>

enum MESSAGES_ID
{

    // id message RMO 1 - 49

    RMO_VOI_TRACK_SETTING_MESSAGE           = 1,                    // Сообщение о проритетности трассы
    RMO_VOI_TRACK_DELETE_MESSAGE            = 2,                    // Сообщение об удалении трассы от РМО
    RMO_VOI_TARGET_POSITION_MESSAGE         = 3,                    // Сообщение с параметрами ЦУ
    RMO_VOI_TARGET_DELETE_MESSAGE           = 4,                    // Сообщение с удалением ЦУ
    RMO_REGFILE_CLOSE                       = 5,                    // Закрытие файла регистрации
    RMO_VOI_DRIVE_TO_ROSITION_MESSAGE       = 6,                    // Сообщение с данными куда смотреть антенне

    RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE     = 9,                    // Сообщение с сектором биологической защиты для ВОИ
    RMO_POI_COMMAND_MESSAGE                 = 10,                   // Сообщение с командой управления
    RMO_RARM_FINISH_CURRENT_FILE            = 11,                   // Завершение записиь текущего файла регистрации
    RMO_RARM_CONNECTION_CONTROL_WITH_POI    = 12,                   // Команда форсированного отключения/подключения реальной первички к РаРМУ
    RMO_RARM_RECORDING_CONTROL              = 13,                   // Команда разрешения/запрета записи файла регистрации
    RMO_OTHER_FORGET_ALL_DATA               = 14,                   // Команда общего сброса данных прошлого режима работы(режима работы РМО)
    RMO_POI_SIMULATOR_PLAY_MESSAGE          = 15,                   // Команда запуска/остановки работы имитатора ПОИ
    RMO_POI_SIMULATOR_CHANGE_FILE_MESSAGE   = 16,                   // Команда смена файла для имитатора ПОИ
    RMO_POI_FC_START_MESSAGE                = 17,                   // Команда запуска функционального контроля для ПОИ
    RMO_POI_SILENCE_MESSAGE                 = 18,                   // Команда отключения обмена с ПОИ, кроме статусных сообщений

    // id message POI 50 - 99

    POI_VOI_FINISH_RAY_MESSAGE              = 51,                   // Сообщение об окончании работы в луче
    POI_KTA_MESSAGE                         = 52,                   // Сообщение о КТА
    POI_POSITION_SENSORS_MESSAGE            = 53,                   // Сообщение с положением антенны в пространстве
    POI_DSP_MESSAGE                         = 54,                   // Сообщение о ДСП
    POI_STATE_MESSAGE                       = 55,                   // Сообщение с состоянием ПОИ
    NAV_MESSAGE                             = 56,                   // Сообщение с навигационными данными
    INCLINOMETR_MESSAGE                     = 57,                   // Сообщение от инклинометра
    MIF380_STATE_MESSAGE                    = 58,                   // Сообщение с состоянием МПЧ380
    M14EP1_STATE_MESSAGE                    = 59,                   // Сообщение с состоянием М14ЕП1 (верхний управляющий блок питания)
    M11PS2_STATE_MESSAGE                    = 60,                   // Сообщение с состоянием М11ПС2 (ПС)
    M12UV1_STATE_MESSAGE                    = 61,                   // Сообщение с состоянием М12УВ1 (ППМ)
    M12HL1_STATE_MESSAGE                    = 62,                   // Сообщение с состоянием М12ХЛ1 (плата интерфейсов)
    M14EP3_STATE_MESSAGE                    = 63,                   // Сообщение с состоянием М14ЕП3 (нижний управляющий блок питания, главный)
    M14EK2_STATE_MESSAGE                    = 64,                   // Сообщение с состоянием М14ЕК2 (блок питания М12УВ1)
    M12UV1_PHASING_RESULT_MESSAGE           = 65,                   // Сообщение с состоянием каналов М12УВ1 (ППМ)

    // id message VOI 100 - 149

    VOI_RMO_AIM_MESSAGE                     = 100,                  // Сообщение о трассе цели
    VOI_RMO_MARK_MESSAGE                    = 101,                  // Сообщение о КТА от ВОИ
    VOI_POI_START_RAYS_MESSAGE              = 102,                  // Сообщение с лучами на следующие 200 мс от ВОИ на ПОИ
    VOI_RMO_RAY_SET_MESSAGE                 = 103,                  // Сообщение с набором лучей на следующий такт
    VOI_RMO_CREATE_SPECIAL_SECTOR_MESSAGE   = 104,                  // Сообщение о создании спец-сектора
    VOI_RMO_DELETE_SPECIAL_SECTOR_MESSAGE   = 105,                  // Сообщение об удалении спец-сектора
    VOI_RMO_TRACK_DELETE_MESSAGE            = 106,                  // Сообщение об удалении трассы от вторички
    VOI_POI_DRIVE_UM_COMMAND_MESSAGE        = 107,                  // Сообщение с командой угломестным приводам
    VOI_POI_DRIVE_AZ_COMMAND_MESSAGE        = 108,                  // Сообщение с командой азимутальному приводу
    VOI_RMO_PRIORITY_SECTOR_MESSAGE         = 109,                  // Сообщение с подтверждеием/отменой высокого приоритета у сектора


    RDDS_OUT_UNIT_POINT_OBJ                 = 140,                  // Сообщение UnitPointObj в RDDS
    RDDS_OUT_UNIT_POINT_OBJ_FOR_DIST        = 141,                  // Сообщение UnitPointObjForDist в RDDS
    RDDS_OUT_UNIT_DSP                       = 142,                  // Сообщение UnitDSP в RDDS
    RDDS_OUT_UNIT_POINT_OBJ_ANS_SIGNAL      = 143,                  // Сообщение UnitPointObjAnsSignal в RDDS
    RDDS_OUT_TRAJECTORY                     = 144,                  // Сообщение Trajectory в RDDS
    RDDS_OUT_PARAM_BEARING_FOR_ANI          = 145,                  // Сообщение ParamBearingForANI в RDDS
    RDDS_IN_TARGET_FOR_RLS                  = 149,                  // Сообщение TargetForRLS из RDDS

    // Sync id 150 - 159
    PC_SYNC_MESSAGE                         = 150,                  // Сообщение синхронизации

    // MOXA id 160 - 169 надо ид в правильном порядке написать
    DEVICES_ADJUSTING_KIT_SET_STATE         = 160,                  // Сообщение установки данных Юстировочного комлпекта в нормальном режиме работы
    DEVICES_ADJUSTING_KIT_GET_STATE         = 161,                  // Сообщение получения данных Юстировочного комлпекта в нормальном режиме работы
    DEVICES_METEO_KIT_GET_MESSAGE           = 162,                  // Сообщение с информацией из модуля обмена с периферийными устройствами(метеостанция)
    DEVICES_MOXA_STATE_MESSAGE              = 163,                  // Сообщение о состоянии преобразователя последовательных интерфейсов, 4xRS-232/422/485(Moxa)
    DEVICES_GEO_STATE_MESSAGE               = 164,                  // Сообщение с координатами от геомодуля ZED-F9P
    DEVICES_ADJUSTING_KIT_BRAR_SET_STATE    = 165,                  // Сообщение с установкой режима БПАР (юстировочный комплект)
    DEVICES_ADJUSTING_KIT_BRAR_GET_STATE    = 166,                  // Сообщение с получением режима БПАР (юстировочный комплект)
    DEVICES_GEO_STATE_REQUEST_MESSAGE       = 167,                  // Сообщение с запросом координт от геомодуля ZED-F9P

    //id message Registration 240 - 249

    REG_CHANGE_FILE                         = 240,                  // Сообщение об изменении файла для воспроизведения
    REG_RREWIND_FILE                        = 241,                  // Сообщение для перемотки файла воспроизведения
    REG_PLAY_ON                             = 242,                  // Сообщение для запуска/остановки воспроизведения
    REG_CURRENT_PLAY_TIME                   = 243,                  // Сообщение о времени воспроизведения из файла
    REG_TRACE_SET_ACTIVATED                 = 244,                  // Сообщение с набором выдаваемых трасс
    REG_EOF                                 = 245,                  // Сообщение об окончании файла


    // id message RARM 250 - 255

    RARM_SYSTEM_CONTROL_MESSAGE             = 253,                  // Сообщение с состоянием подсистем
    ALL_SYSTEM_HANDSHAKE                    = 255                   // Сообщение о рукопожатии с ММИРом
};

enum SYSTEMS_ID
{
    POI_SYS_ID = 0,                                     // Идентификатор системы ПОИ рабочего блока, платы Master
    VOI_SYS_ID = 1,
    RMO_SYS_ID = 2,
    RARM_SYS_ID = 3,
    RDDS_SYS_ID = 4,
    POI_SYS_ID_INCOMMING_CONN = 5,                      // Идентификатор входящего соединения от системы ПОИ с рабочего блока, платы MASTER
    PROGRAM_SIMULATOR_SYS_ID = 6,
    REGISTRATION_SYS_ID = 7,                            // tcpSender
    POI_SIMULATOR = 8,                                  // Имитатор первички
    PROCESS_CONTROLLER = 9,                             // Программа контроля и синхронизации всех модулей
    POI_SYS_ID_SLAVE_WORK = 10,                         // Идентификатор системы ПОИ рабочего блока, платы Slave
    POI_SYS_ID_MASTER_RESERVE = 11,                     // Идентификатор системы ПОИ резервного блока, платы Master
    POI_SYS_ID_SLAVE_RESERVE = 12,                      // Идентификатор системы ПОИ резервного блока, платы Slave
    POI_SYS_ID_INCOMMING_CONN_MASTER_RESERVE = 13,      // Идентификатор входящего соединения от системы ПОИ с резервного блока, платы MASTER
    DEVICE_MANAGER = 14,                                // Идентификатор MOXA
};

typedef struct timeval64                                    // Время
{
    quint64 secs;                        // Cекунды
    quint64 usecs;                       // Микросекунды
} timeval64;

//DEVICES_ADJUSTING_KIT_SET_STATE = 160
//DEVICES_ADJUSTING_KIT_GET_STATE = 161
// Отправка состояний в юстировочный комлект из РМО
struct DevicesAdjustingKitMessage
{
    timeval64 sTimeMeasurement; // Время создание сообщения
    quint32 FvcoRx; // Рабочая точка Fvco для частоты Rx: [Герцы]
    quint32 FvcoTx; // Рабочая точка Fvco для частоты Rx: [Герцы]
    int DoplerFrequency ; // Частота Доплера (по желанию) можеть быть меньше 0 [Герцы]
    quint32 Distance; // Дальность ответного сигнала [Метры]
    quint32 DistanceToLocator; //Дальность от ответчика до локатора [Метры]
    float GAIN_TX;//Усиление TX, <=31.5 [Децибелы]
    float GAIN_RX;//Усиление RX, <=31.5 [Децибелы]
    quint8 Attenuator;//Установка ослабления [Децибелы]
    quint8 WorkMode;//Режимы работы: 0 - все отключено, 1(true) - ответчик включен, 2 - режим БПАР
    quint8 state; // 0 - нет связи, 1 - всё штатно работает, 2 - время ожидания ответа истекло, 3 - сброс данных
};


//ID: DEVICES_METEO_KIT_GET_MESSAGE = 162
// Сообщение о состоянии метеостанции
struct DevicesMeteoKitGetMessage
{
    timeval64 sTimeMeasurement; // Время создание сообщения
    float pressure;
    float temperature;
    float wet;
    uint8_t state;  // 0 - нет связи, 1 - сообщение пришло, контрольная сумма не совпала, 2 - отказ метеостанции от исполнения команды,
    // 3 - неверные данные, 4 - всё штатно работает, 5 - время ожидания ответа истекло
};

// ID: DEVICES_MOXA_STATE_MESSAGE  = 163,
// Сообщение о состоянии MOXA
struct DevicesMoxaStateMessage
{
    timeval64 sTimeMeasurement; // Время создание сообщения
    quint8 state; // 0 - нет связи, 1 - всё штатно работает
    quint8 stateCK; // 0 - нет связи, 1 - всё штатно работает
};

// ID: DEVICES_GEO_STATE_MESSAGE  = 164,
// Сообщение с координатами от геомодуля ZED-F9P
struct DevicesGeoStateMessage
{
    timeval64 sTimeMeasurement; // Время создание сообщения
    quint8 state; // 0 - нет связи, 1 - всё штатно работает, 2 - время ожидания истекло, 3 - нет антенны, 4 - неверные данные геодатчика
    float latitude;// Широта в формате "ГГ.ГГГГГГ" + север, - юг
    float longtitude;// Долгота в формате "ГГГ.ГГГГГГ" + восток, - запад
    float height;// высота над уровнем моря (метры)
};

// ID: DEVICES_ADJUSTING_KIT_BRAR_SET_STATE = 165,
// ID: DEVICES_ADJUSTING_KIT_BRAR_GET_STATE = 166,
// Отправка состояний в юстировочный комлект из РМО (режим БПАР)
struct DevicesBparAdjustingKitMessage
{
    timeval64 sTimeMeasurement; // Время создание сообщения
    quint8 foId; // Текущий индекс Fo (0...5)
    bool isLcm; // Наличие ЛЧМ(Линейно-частотной модуляции) (0 - выключен, 1 - включен)
    quint8 tksIndex ; // Индекс Тк (0..2) (ЛЧМ включен - всегда 0)
    bool hasThreshold; // Включить порог
    quint16 threshold; // Порог сигнала
    int answerDelay;// Задержка сигнала (0..5999)
    quint32 DistanceToLocator; //Дальность от ответчика до локатора [Метры]
    quint8 WorkMode;// Режимы работы: 0 - все отключено, 1 - ответчик, 2 - БПАР
    quint8 state; // 0 - нет связи, 1 - всё штатно работает, 2 - время ожидания ответа истекло, 3 - сброс данных
};

#endif // DATAMESSAGE_H
