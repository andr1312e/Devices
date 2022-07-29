#ifndef MESSAGESTEST_H
#define MESSAGESTEST_H
#include <QtTest>

#include "defines.h"

#include "../program/messageCreatrors/ustirovmessagegegetter.h"
#include "../program/messageRepositories/ustrirovmessagerepository.h"
#include "../program/messageRepositories/geomessagerepository.h"

#include "../program/messageCreatrors/ustirovmessagesender.h"

class MessagesTest : public QObject
{
    Q_OBJECT
public:
    explicit MessagesTest();
    ~MessagesTest();
Q_TESTS:
    void WrongMessage();
    void TestFirstMessage();
    void TestSecondMessage();
    void TestThirdMessage();
    void TestFourthMessage();
    void TestFiveMessage();
    void TestSixMessage();
    void TestNineMessage();
    void TestBparMessage();
};

#endif // MESSAGESTEST_H
