#ifndef TEST_PORT_TYPE_H
#define TEST_PORT_TYPE_H

#include "autotest.h"

class TestPortType : public QObject
{
    Q_OBJECT
private slots:
    void testInt();
    void testDate();

    void testDeduceBool();
    void testDeduceChar();
    void testDeduceInt();
    void testDeduceIntOneChar();
    void testDeduceDouble();
    void testDeduceDate();
    void testDeduceDateTime();
    void testDeduceTime();
    void testDeduceQString();

    void testDeduceBoolVector();
    void testDeduceIntVector();
    void testDeduceDoubleVector();
    void testDeduceDateVector();
};

DECLARE_TEST(TestPortType)

#endif
