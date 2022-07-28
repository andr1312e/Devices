#include <QtTest>
#include "../program/messageCreatrors/ustirovmessagegegetter.h"
#include "../program/messageRepositories/ustrirovmessagerepository.h"
#include "../program/messageRepositories/geomessagerepository.h"

class Test_MessageGetter : public QObject
{
    Q_OBJECT

public:
    Test_MessageGetter();
    ~Test_MessageGetter();

private slots:
    void TestZeroMessage();
private:

};

Test_MessageGetter::Test_MessageGetter()
{

}

Test_MessageGetter::~Test_MessageGetter()
{

}

void Test_MessageGetter::TestZeroMessage()
{
    UstrirovMessageRepository ustirovRep;
    GeoMessageRepository geoRep;
    UstirovMessageGetter getter(30625000, 400000, ustirovRep, geoRep, Q_NULLPTR);
    const QByteArray message(QByteArrayLiteral("\x07\x01\x00\x83\x0D\x99\x99\x99"));
    const bool isFile = getter.FillDataIntoStructFromMessage(message);
    QCOMPARE(isFile, true);
}

QTEST_APPLESS_MAIN(Test_MessageGetter)

#include "tst_tests.moc"
