#include <QVector>
#include <base/convert.h>
#include "exception_expectation.h"
#include "test_convert_vector.h"

using namespace base;

void TestConvertVector::testIsInstance() {
    bool ok;

    ok = isInstance<QVector<int>, QVector>{};
    QVERIFY(ok);

    ok = isInstance<int, QVector>{};
    QVERIFY(!ok);
}

void TestConvertVector::testBool() {
    bool excepted(false);
    QVector<bool> a, b, c;
    QVector<int>  d;

    b << true << false << true;
    c << false << false << true << true;
    d << 0 << 0 << 7 << 8;

    try {
        a = convert<QVector<bool>>(b);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(a, b);

    try {
        a = convert<QVector<bool>>(d);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(a, c);
}

void TestConvertVector::testScalarToVector() {
    bool excepted(false);
    QVector<int>     a, b = {113};
    QVector<QString> c, d = {"17"};

    try {
        a = convert<QVector<int>>(113);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(a, b);

    try {
        c = convert<QVector<QString>>(17);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(c, d);
}

template <class T, class U> T convertScalarVec(QVector<U> x) {
    if (x.size() != 1)
        ThrowException("A vector must have exactly one element to be converted to a scalar").value(x.size());
    return convert<T,U>(x.at(0));
}

template <class T> T convert(QVector<int    > x)  { return convertScalarVec<T, int    >(x); }
template <class T> T convert(QVector<QString> x)  { return convertScalarVec<T, QString>(x); }

//QString convert(QVector<int> x)        { return convertScalarVec<QString, int    >(x); }
//QString convert(QVector<QString> x)    { return convertScalarVec<QString, QString>(x); }

void TestConvertVector::testVectorToScalar() {
    bool excepted(false);
    int a;
    QString c;
    QVector<int>     b = {113};
    QVector<QString> d = {"17"};

    try {
        a = convert<int>(b);
        c = convert<QString>(b);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(a, 113);
    QCOMPARE(c, QString("113"));

    try {
        a = convert<int>(d);
        c = convert<QString>(d);
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(a, 17);
    QCOMPARE(c, QString("17"));
}
