#ifndef TEST_VALUE_H
#define TEST_VALUE_H

#include "autotest.h"

class TestValue : public QObject
{
    Q_OBJECT
private slots:
    void testBoolTyped();
    void testInitializeByValue();
    void testInitializeByPointer();
    void testAssignmentByValue();
    void testAssignmentByPointer();
    void testAssignmentTwice();
    void testAssignmentToUninitialized();
    void testBool();
    void testInt();
};

DECLARE_TEST(TestValue)

#endif
