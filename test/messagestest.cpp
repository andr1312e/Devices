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
    //Проверяем все ослабления
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
        const QByteArray message(QByteArrayLiteral("\x07\x04\x08\x07"));
        const bool canParce = getter.FillDataIntoStructFromMessage(message);
        QVERIFY(canParce);
        QCOMPARE(ustirovRep.GetNormalMessage().GAIN_TX, 4);
        QCOMPARE(ustirovRep.GetNormalMessage().GAIN_RX, 3.5);
    }
}

void MessagesTest::TestSixMessage()
{

}
