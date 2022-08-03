#include <limits>
#include <base/convert.h>
#include <base/port_type.h>
#include "test_port_type.h"

using std::numeric_limits;
using namespace base;

void TestPortType::testInt() {
    QCOMPARE(typeOf<int>(), Int);
}

void TestPortType::testDate() {
    QCOMPARE(typeOf<QDate>(), Date);
}

void TestPortType::testDeduceBool() {
    PortType type = deducePortTypeFromString("TRUE");
    QCOMPARE(type, Bool);
}


void TestPortType::testDeduceChar() {
    QCOMPARE(deducePortTypeFromString("X"), Char);
    QCOMPARE(deducePortTypeFromString(" "), Char);
}

void TestPortType::testDeduceInt(){
    PortType type = deducePortTypeFromString("123");
    QCOMPARE(type, Int);
}

void TestPortType::testDeduceIntOneChar(){
    PortType type = deducePortTypeFromString("7");
    QCOMPARE(type, Int);
}

void TestPortType::testDeduceDouble() {
    PortType type = deducePortTypeFromString("3.14");
    QCOMPARE(type, Double);
}

void TestPortType::testDeduceDate() {
    QCOMPARE(deducePortTypeFromString("31/12/2001"), Date);
    QCOMPARE(deducePortTypeFromString("2001/12/31"), Date);
    QCOMPARE(deducePortTypeFromString("/12/31/2001"), Date);
}

void TestPortType::testDeduceDateTime() {
    QCOMPARE(deducePortTypeFromString("31/12/2001 9:35"), DateTime);
}

void TestPortType::testDeduceTime() {
    QCOMPARE(deducePortTypeFromString("9:35"), Time);
}

void TestPortType::testDeduceQString() {
    QCOMPARE(deducePortTypeFromString(""), String);
    QCOMPARE(deducePortTypeFromString("True"), String);
    QCOMPARE(deducePortTypeFromString("(a b "), String);
    QCOMPARE(deducePortTypeFromString("a38"), String);
}

void TestPortType::testDeduceBoolVector() {
    QCOMPARE(deducePortTypeFromString("(TRUE FALSE)"), BoolVector);
}
void TestPortType::testDeduceIntVector() {
    QCOMPARE(deducePortTypeFromString("(7 13 -5 0)"), IntVector);
}

void TestPortType::testDeduceDoubleVector() {
    QCOMPARE(deducePortTypeFromString("(10.0 30.0 20.0)"), DoubleVector);
    QCOMPARE(deducePortTypeFromString("(7 13 -5 0.2)"), DoubleVector);
}

void TestPortType::testDeduceDateVector() {
    QCOMPARE(deducePortTypeFromString("(31/12/2001)"), DateVector);
    QCOMPARE(deducePortTypeFromString("(31/12/2001 2001/12/31 /12/31/2001)"), DateVector);
    QCOMPARE(deducePortTypeFromString("(31/12/2001 2001/12/31 /12/31/2001 x)"), StringVector);
}

