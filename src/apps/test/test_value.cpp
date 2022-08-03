#include <iostream>
#include <QString>
#include <base/value.h>
#include "exception_expectation.h"
#include "test_value.h"

using namespace base;

void TestValue::testBoolTyped() {
    ValueTyped<bool> val;
    const bool *x;
    bool excepted(false);

    try {
        val.initialize(8);
        QCOMPARE(val.value<bool>(), true);
        QCOMPARE(val.value<int>(), 1);
        QCOMPARE(val.value<QString>(), QString("1"));

        x = val.constPtr();
        QCOMPARE(*x, true);

        val.changeValue(QString("FALSE"));
        QCOMPARE(val.value<bool>(), false);

        val.changeValue(QString("0"));
        QCOMPARE(val.value<bool>(), false);

        val.changeValue(QString("TRUE"));
        QCOMPARE(val.value<bool>(), true);

        val.changeValue(QString("1"));
        QCOMPARE(val.value<bool>(), true);

    }
    UNEXPECTED_EXCEPTION;

    try {
        val.changeValue(QString("abc"));
    }
    EXPECTED_EXCEPTION;
}


void TestValue::testInitializeByValue() {
    Value a, b;

    a.initialize(17);
    b.initialize(3.12);
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.as<int>(), 17);
    QCOMPARE(b.as<double>(), 3.12);

    const int    *x = a.constPtr<int>();
    const double *y = b.constPtr<double>();

    a.changeValue(3.12);
    b.changeValue(17);
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.as<int>(), 3);
    QCOMPARE(b.as<double>(), 17.0);

    QCOMPARE(*x, 3);
    QCOMPARE(*y, 17.0);
}

void TestValue::testInitializeByPointer() {
    int i = 17;
    double k = 3.12;
    Value a, b;

    a.initialize(&i);
    b.initialize(&k);
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.as<int>(), 17);
    QCOMPARE(b.as<double>(), 3.12);

    const int    *x = a.constPtr<int>();
    const double *y = b.constPtr<double>();

    a.changeValue(3.12);
    b.changeValue(17);
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(i, 3);
    QCOMPARE(k, 17.0);
    QCOMPARE(a.as<int>(), 3);
    QCOMPARE(b.as<double>(), 17.0);

    QCOMPARE(*x, 3);
    QCOMPARE(*y, 17.0);
}

void TestValue::testAssignmentByValue() {
    Value a, b;
    a = 17;
    b = 3.12;
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.as<int>(), 17);
    QCOMPARE(b.as<double>(), 3.12);
}

void TestValue::testAssignmentByPointer() {
    int i = 17;
    double k = 3.12;
    Value a, b;
    a = &i;
    b = &k;
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.as<int>(), 17);
    QCOMPARE(b.as<double>(), 3.12);
}

void TestValue::testAssignmentTwice() {
    Value a;
    a = 117;
    QCOMPARE(a.as<int>(), 117);
    a.changeValue(118);
    QCOMPARE(a.as<int>(), 118);
    a = 119;
    QCOMPARE(a.as<int>(), 119);

    // Changing a Value by assignment does not thange the type!
    a = 3.12;
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(a.as<int>(), 3);
}

void TestValue::testAssignmentToUninitialized() {
    Value a, b;
    a = "abc";
    b = a;
    QCOMPARE(a.type(), Value::Type::String);
    QCOMPARE(b.type(), Value::Type::String);
    QCOMPARE(a.as<QString>(), QString("abc"));
    QCOMPARE(b.as<QString>(), QString("abc"));
}

void TestValue::testBool() {
    bool excepted(false);
    Value val;
    bool x = false;
    const bool *y;
    try {
        val.initialize(&x);

        val.changeValue(8);
        QCOMPARE(val.as<bool>(), true);
        QCOMPARE(x, true);
        QCOMPARE(val.as<int>(), 1);
        QCOMPARE(val.as<QString>(), QString("1"));

        y = val.constPtr<bool>();
        QCOMPARE(*y, true);

        val.changeValue(QString("FALSE"));
        QCOMPARE(val.as<bool>(), false);
        QCOMPARE(x, false);
    }
    UNEXPECTED_EXCEPTION;

    try {
        val.changeValue(QString("abc"));
    }
    EXPECTED_EXCEPTION;
}

void TestValue::testInt() {
    bool excepted(false);
    Value val;

    int x = 117;
    val.initialize(&x);
    QCOMPARE(val.as<int>(), 117);

    const int *y = val.constPtr<int>();
    QCOMPARE(*y, 117);

    try {
        val.constPtr<double>();
    }
    EXPECTED_EXCEPTION;

    Value val2;
    val.initialize(42);
    QCOMPARE(val.type(), Value::Type::Int);
    QCOMPARE(val.as<int>(), 42);

    Value a, b;

    int u = 17;
    double v = 3.12;
    a.initialize(&u);
    b.initialize(&v);
    QCOMPARE(a.type(), Value::Type::Int);
    QCOMPARE(b.type(), Value::Type::Double);
    QCOMPARE(a.as<int>(), 17);
    QCOMPARE(b.as<double>(), 3.12);
}
