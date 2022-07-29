#include "messagestest.h"

QTEST_APPLESS_MAIN(MessagesTest)

MessagesTest::MessagesTest()
{

}

MessagesTest::~MessagesTest()
{

}

void MessagesTest::WrongMessage()
{
    UstrirovMessageRepository ustirovRep;
    GeoMessageRepository geoRep;
    const int f = 30625000;
    const int fref = 40000000;
    UstirovMessageGetter getter(f, fref, ustirovRep, geoRep, Q_NULLPTR);
    //Перехватываем исключение
    const QByteArray message(QByteArrayLiteral("\x07\x99\x00\x00\x84\x01\x99\x99"));
    QVERIFY_EXCEPTION_THROWN(getter.FillDataIntoStructFromMessage(message), std::runtime_error);
}

void MessagesTest::TestRestartMessage()
{
    const int f = 30625000;
    const int fref = 40000000;
    UstirovMessageSender sender(Q_NULLPTR, f, fref);
    const QByteArray message=sender.CreateRestartCommand();
    QCOMPARE(message, QByteArrayLiteral("\x0d\x5a"));
}

void MessagesTest::TestZeroMessage()
{
    const int f = 30625000;
    const int fref = 40000000;
    UstirovMessageSender sender(Q_NULLPTR, f, fref);
    const QByteArray message=sender.CreateZeroCommand();
    QCOMPARE(message, QByteArrayLiteral("\x00\x00"));
}

void MessagesTest::TestFirstMessage()
{
    UstrirovMessageRepository ustirovRep;
    GeoMessageRepository geoRep;
    const int f = 30625000;
    const int fref = 40000000;
    UstirovMessageGetter getter(f, fref, ustirovRep, geoRep, Q_NULLPTR);

    //2725
    QBENCHMARK
    {
        const QByteArray message(QByteArrayLiteral("\x07\x01\x00\x84\x01\x99\x99\x00"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QVERIFY(canParce);
        const int diff = qAbs(2.725e+9 - ustirovRep.GetNormalMessage().FvcoRx);
        QVERIFY(diff < 20);
    }
    //Неверный размер
    {
        const QByteArray message(QByteArrayLiteral("\x07\x01\x00\x84\x01\x99\x99"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QCOMPARE(canParce, false);
    }

    //Проверяем все рабочие точки
    {

        double workPoint = 2.695e+9;
        UstirovMessageSender sender(Q_NULLPTR, f, fref);
        for (int i = 0; i < 30; ++i)
        {
            QByteArray message = sender.CreateFirstCommand(workPoint);
            message.prepend(static_cast<char>(0x07));
            const bool canParce = getter.FillDataIntoStructFromMessage(message);
            QVERIFY(canParce);
            QVERIFY(qAbs(workPoint - ustirovRep.GetNormalMessage().FvcoRx) <= 25);
            workPoint = workPoint + 1.e+6;
        }
    }    //Перехватываем исключение
    {
        const QByteArray message(QByteArrayLiteral("\x07\x99\x00\x00\x84\x01\x99\x99"));
        QVERIFY_EXCEPTION_THROWN(getter.FillDataIntoStructFromMessage(message), std::runtime_error);
    }
}

void MessagesTest::TestSecondMessage()
{
    UstrirovMessageRepository ustirovRep;
    GeoMessageRepository geoRep;
    const int f = 30625000;
    const int fref = 40000000;
    UstirovMessageGetter getter(f, fref, ustirovRep, geoRep, Q_NULLPTR);

    //2725 рабочая точка
    {
        const QByteArray message(QByteArrayLiteral("\x07\x02\x00\x84\x01\x99\x99\x00"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QVERIFY(canParce);
        const int diff = qAbs(2.725e+9 - ustirovRep.GetNormalMessage().FvcoTx);
        QVERIFY(diff < 20);
    }
    //Неверный размер
    {
        const QByteArray message(QByteArrayLiteral("\x07\x01\x00\x84\x01\x99\x99"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QCOMPARE(canParce, false);
    }

    //Проверяем все рабочие точки
    {
        double workPoint = 2.695e+9;
        UstirovMessageSender sender(Q_NULLPTR, f, fref);
        for (int i = 0; i < 30; ++i)
        {
            QByteArray message = sender.CreateSecondCommand(workPoint);
            message.prepend(static_cast<char>(0x07));
            const bool canParce = getter.FillDataIntoStructFromMessage(message);
            QVERIFY(canParce);
            QVERIFY(qAbs(workPoint - ustirovRep.GetNormalMessage().FvcoTx) <= 25);
            workPoint += 1.e+6;
        }
    }

    //Перехватываем исключение
    {
        const QByteArray message(QByteArrayLiteral("\x07\x99\x00\x00\x84\x01\x99\x99"));
        QVERIFY_EXCEPTION_THROWN(getter.FillDataIntoStructFromMessage(message), std::runtime_error);
    }
}

void MessagesTest::TestThirdMessage()
{
    UstrirovMessageRepository ustirovRep;
    ustirovRep.SetDistanceToLocator(350);
    GeoMessageRepository geoRep;
    const int f = 30625000;
    const int fref = 40000000;
    UstirovMessageGetter getter(f, fref, ustirovRep, geoRep, Q_NULLPTR);

    //Обрабатываем сообщение
    {
        const QByteArray message(QByteArrayLiteral("\x07\x03\x2f\xfb"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QVERIFY(canParce);
        const int distDiff = 60490 - ustirovRep.GetNormalMessage().Distance;
        const int diff = qAbs(distDiff);
        QVERIFY(diff < 5);
    }
    //Неверный размер
    {
        const QByteArray message(QByteArrayLiteral("\x07\x03\x2f\x2f\x2f"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QCOMPARE(canParce, false);
    }
    //Проверяем все дальности 350 м от локатора
    {
        int distance = 1000;
        UstirovMessageSender sender(Q_NULLPTR, f, fref);
        for (int i = 0; i < 150; ++i)
        {
            QByteArray message = sender.CreateThirdCommand(distance, 350);
            message.prepend(static_cast<char>(0x07));
            const bool canParce = getter.FillDataIntoStructFromMessage(message);
            QVERIFY(canParce);
            const int distDiff = distance - ustirovRep.GetNormalMessage().Distance;
            const int diff = qAbs(distDiff);
            QVERIFY(diff < 5);
            distance += 100;
        }
    }

    //Проверяем все дальности от 400 до 1км от локатора
    for (int distanceToLocator = 400; distanceToLocator < 1000; distanceToLocator += 20)
    {
        ustirovRep.SetDistanceToLocator(distanceToLocator);
        int distance = 1052;
        UstirovMessageSender sender(Q_NULLPTR, f, fref);
        for (int i = 0; i < 440; ++i)
        {
            QByteArray message = sender.CreateThirdCommand(distance, distanceToLocator);
            message.prepend(static_cast<char>(0x07));
            const bool canParce = getter.FillDataIntoStructFromMessage(message);
            QVERIFY(canParce);
            const int distDiff = distance - ustirovRep.GetNormalMessage().Distance;
            const int diff = qAbs(distDiff);
            QVERIFY(diff < 5);
            distance += 32;
        }
    }
    //Если дистанция которую мы хотим мельше чем дистанция до локатора то будет ошибка в формулах
}

void MessagesTest::TestFourthMessage()
{
    UstrirovMessageRepository ustirovRep;
    GeoMessageRepository geoRep;
    UstirovMessageGetter getter(0, 0, ustirovRep, geoRep, Q_NULLPTR);
    //Обрабатываем сообщение
    {
        const QByteArray message(QByteArrayLiteral("\x07\x04\x08\x07"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QVERIFY(canParce);
        QCOMPARE(ustirovRep.GetNormalMessage().GAIN_TX, 4);
        QCOMPARE(ustirovRep.GetNormalMessage().GAIN_RX, 3.5);
    }
    //Проверяем что создается то же
    {
        const double  GAIN_TX = 2;
        const double GAIN_RX = 4.5;
        UstirovMessageSender sender(nullptr, 0, 0);
        const QByteArray message = sender.CreateFourthCommand(GAIN_TX, GAIN_RX);
        QCOMPARE(message, QByteArrayLiteral("\x04\x04\x09"));
    }
    //Неверный размер
    {
        const QByteArray message(QByteArrayLiteral("\x07\x04\x2f\x2f\x2f"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QCOMPARE(canParce, false);
    }
    //Проверяем все усиления
    UstirovMessageSender sender(nullptr, 0, 0);
    for (double i = 0.0; i <= 31.5; i += 0.5)
    {
        for (double j = 0.0; j <= 31.5; j += 0.5)
        {
            QByteArray message = sender.CreateFourthCommand(i, j);
            message.prepend(static_cast<char>(0x07));
            const bool canParce = getter.FillDataIntoStructFromMessage(message);
            QVERIFY(canParce);
            QCOMPARE(ustirovRep.GetNormalMessage().GAIN_TX, i);
            QCOMPARE(ustirovRep.GetNormalMessage().GAIN_RX, j);
        }
    }
}

void MessagesTest::TestFiveMessage()
{
    UstrirovMessageRepository ustirovRep;
    GeoMessageRepository geoRep;
    UstirovMessageGetter getter(0, 0, ustirovRep, geoRep, Q_NULLPTR);
    //Обрабатываем сообщение
    {
        const QByteArray message(QByteArrayLiteral("\x07\x05\x00"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QVERIFY(canParce);
        QCOMPARE(ustirovRep.GetNormalMessage().Attenuator, 0);
    }
    //Проверяем что создается то же
    {
        UstirovMessageSender sender(nullptr, 0, 0);
        const int attenuator = 9;
        const QByteArray message = sender.CreateFiveCommand(attenuator);
        QCOMPARE(message, QByteArrayLiteral("\x05\x08"));
    }
    //Неверный размер
    {
        const QByteArray message(QByteArrayLiteral("\x07\x05\x2f\x2f\x2f\x2f\x2f\x2f\x2f"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QCOMPARE(canParce, false);
    }
    //Проверяем все ослабления
    UstirovMessageSender sender(nullptr, 0, 0);
    const QList<quint8> attenuatorKeys = atteniatorTable.keys();
    for (const quint8 attenuator : attenuatorKeys)
    {
        QByteArray message = sender.CreateFiveCommand(attenuator);
        message.prepend(static_cast<char>(0x07));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QVERIFY(canParce);
        QCOMPARE(ustirovRep.GetNormalMessage().Attenuator, attenuator);
    }
    //Проверяем что не будет работать
    {
        const quint8 invalidKey = 14;
        QVERIFY_EXCEPTION_THROWN(sender.CreateFiveCommand(invalidKey), std::runtime_error);
    }
}

void MessagesTest::TestSixMessage()
{
    UstrirovMessageRepository ukitRepository;
    GeoMessageRepository geoRepository;
    UstirovMessageGetter getter(0, 0, ukitRepository, geoRepository, nullptr);
    //Проверяем как создаются сообщения
    {
        UstirovMessageSender sender(nullptr, 0, 0);
        const QByteArray messageOff = sender.CreateSixCommand(0);
        QCOMPARE(messageOff, QByteArrayLiteral("\x06\x00"));
        const QByteArray messageOn = sender.CreateSixCommand(1);
        QCOMPARE(messageOn, QByteArrayLiteral("\x06\x01"));
    }
    //Проверяем что не будет работать c несуществующим режимом
    {
        UstirovMessageSender sender(nullptr, 0, 0);
        QVERIFY_EXCEPTION_THROWN(sender.CreateSixCommand(2), std::runtime_error);
    }
    //Неверный размер
    {
        const QByteArray message(QByteArrayLiteral("\x07\x06\x2f\x2f"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QCOMPARE(canParce, false);
    }
    //Проверяем парсинг обратный
    {
        const bool canParceFirst = getter.FillDataIntoStructFromMessage(QByteArrayLiteral("\x07\x06\x00"));
        QVERIFY(canParceFirst);
        QCOMPARE(ukitRepository.GetNormalMessage().WorkMode, 0);
        const bool canParceSecond = getter.FillDataIntoStructFromMessage(QByteArrayLiteral("\x07\x06\x01"));
        QVERIFY(canParceSecond);
        QCOMPARE(ukitRepository.GetNormalMessage().WorkMode, 1);
    }
}

void MessagesTest::TestNineMessage()
{
    UstrirovMessageRepository ukitRepository;
    GeoMessageRepository geoRepository;
    UstirovMessageGetter getter(0, 0, ukitRepository, geoRepository, nullptr);
    //Проверяем как создаются сообщения
    {
        UstirovMessageSender sender(nullptr, 0, 0);
        const QByteArray message = sender.CreateNineCommand(99);
        QCOMPARE(message, QByteArrayLiteral("\x09\x00\x00\x63"));
    }
    //Неверный размер
    {
        const QByteArray message(QByteArrayLiteral("\x07\x09\x2f\x2f\x2f\x2f"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QCOMPARE(canParce, false);
    }
    //Парсинг
    {
        QVERIFY(getter.FillDataIntoStructFromMessage(QByteArrayLiteral("\x07\x09\x00\x00\x63")));
        QCOMPARE(ukitRepository.GetNormalMessage().DoplerFrequency, 99);
    }
}

void MessagesTest::TestBparMessage()
{
    UstrirovMessageRepository ukitRepository;
    ukitRepository.SetDistanceToLocator(350);
    GeoMessageRepository geoRepository;
    UstirovMessageGetter getter(30625000, 0, ukitRepository, geoRepository, nullptr);
    //Проверяем как создаются сообщения
    {
        UstirovMessageSender sender(nullptr, 30625000, 0);
        DevicesBparAdjustingKitMessage messageBpar;
        messageBpar.DistanceToLocator = 350;
        messageBpar.foId = 4;
        messageBpar.answerDelay = 250;
        messageBpar.hasThreshold = false;
        messageBpar.isLcm = false;
        messageBpar.tksIndex = 1;
        messageBpar.threshold=1;
        const QByteArray message = sender.CreateBparCommand(messageBpar);
        QCOMPARE(message, QByteArrayLiteral("\x06\x05\x14\x00\x10\x00\x01"));
    }
    //Неверный размер
    {
        const QByteArray message(QByteArrayLiteral("\x07\x06\x05\x24\x00\x06\x10"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QCOMPARE(canParce, false);
    }
    //Парсинг
    {
        const bool res = getter.FillDataIntoStructFromMessage(QByteArrayLiteral("\x07\x06\x05\x14\x00\x10\x00\x00"));
        QVERIFY(res);
        QCOMPARE(ukitRepository.GetBparMessage().isLcm, false);
        QCOMPARE(ukitRepository.GetBparMessage().tksIndex, 1);
        QCOMPARE(ukitRepository.GetBparMessage().foId, 4);
        QCOMPARE(ukitRepository.GetBparMessage().hasThreshold, false);
        QCOMPARE(ukitRepository.GetBparMessage().threshold, 0);
        const int diff = qAbs(ukitRepository.GetBparMessage().answerDelay - 250);
        QVERIFY( diff < 5);
    }
    //Парсинг2
    {
        const bool res = getter.FillDataIntoStructFromMessage(QByteArrayLiteral("\x07\x06\x05\x4a\x00\x9e\x01\x90"));
        QVERIFY(res);
        QCOMPARE(ukitRepository.GetBparMessage().isLcm, true);
        QCOMPARE(ukitRepository.GetBparMessage().tksIndex, 0);
        QCOMPARE(ukitRepository.GetBparMessage().hasThreshold, true);
        QCOMPARE(ukitRepository.GetBparMessage().foId, 2);
        QVERIFY(ukitRepository.GetBparMessage().hasThreshold);
        QCOMPARE(ukitRepository.GetBparMessage().threshold, 400);
        const int diff = qAbs(ukitRepository.GetBparMessage().answerDelay - 1000);
        QVERIFY( diff < 5);
    }
    //Парсинг всех задержек
    {
        UstirovMessageSender sender(nullptr, 30625000, 0);
        DevicesBparAdjustingKitMessage messageBpar;
        messageBpar.DistanceToLocator = 350;
        messageBpar.foId = 4;
        messageBpar.hasThreshold = false;
        messageBpar.isLcm = false;
        messageBpar.tksIndex = 1;
        messageBpar.threshold = 1;
        for (int i = 250; i <= 35000; i += 50)
        {
            messageBpar.answerDelay = i;
            QByteArray message = sender.CreateBparCommand(messageBpar);
            message.prepend(static_cast<char>(0x07));
            const bool result = getter.FillDataIntoStructFromMessage(message);
            const int diff = qAbs(ukitRepository.GetBparMessage().answerDelay - i);
            QVERIFY(result);
            QVERIFY( diff >= 0 && diff < 6);
        }

    }
}
