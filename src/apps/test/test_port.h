#ifndef TEST_PORT_H
#define TEST_PORT_H

#include "autotest.h"

#define CONV(X,Y,Z) \
    void test##X##To##Y##ScalarToScalar##Z(); \
    void test##X##To##Y##VectorToScalar##Z(); \
    void test##X##To##Y##ScalarToVector##Z(); \
    void test##X##To##Y##VectorToVector##Z();

#define CONV_ALL(Y) \
    CONV(Bool, Y, ) \
    CONV(Char, Y, ) \
    CONV(Int, Y, ) \
    CONV(LongInt, Y, ) \
    CONV(LongLongInt, Y, ) \
    CONV(Float, Y, ) \
    CONV(Double, Y, ) \
    CONV(LongDouble, Y, ) \
    CONV(Date, Y, ) \
    CONV(Time, Y, ) \
    CONV(DateTime, Y, ) \
    CONV(String, Y, )




class TestPort : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    // Scalar
    void testFixedInt();
    void testPtrInt();
    void testFixedString();
    void testPtrString();
    // Vector
    void testIntVector();
    void testStringVector();
    // Old-fashioned C-string
    void testFixedCString();
    // US Date
    void testUSDate();

    // Reset
    void testResetBool();
    void testResetBoolVector();
    void testResetLongInt();
    void testResetString();

    // All conversion combinations
    CONV_ALL(Bool)
    CONV_ALL(Char)
    CONV_ALL(Int)
    CONV_ALL(LongInt)
    CONV_ALL(LongLongInt)
    CONV_ALL(Float)
    CONV_ALL(Double)
    CONV_ALL(LongDouble)
    CONV_ALL(Date)
    CONV_ALL(Time)
    CONV_ALL(DateTime)

    // Overflows
    CONV(LongLongInt, Int, Overflow)
    CONV(Float, Int, Overflow)
    CONV(Double, Int, Overflow)
    CONV(LongDouble, Int, Overflow)
    CONV(LongLongInt, LongInt, Overflow)
    CONV(Float, LongInt, Overflow)
    CONV(Double, LongInt, Overflow)
    CONV(LongDouble, LongInt, Overflow)
    CONV(LongDouble, Float, Overflow)
    CONV(LongDouble, Double, Overflow)
    CONV(Float, LongLongInt, Overflow)
    CONV(Double, LongLongInt, Overflow)
    CONV(LongDouble, LongLongInt, Overflow)
    CONV(String, Date, Illegal)
    CONV(String, Time, Illegal)
    CONV(String, DateTime, Illegal)

    // Syntax errors
    CONV(String, Bool, SyntaxError)
    CONV(String, Char, SyntaxError)
    CONV(String, Int, SyntaxError)
    CONV(String, LongInt, SyntaxError)
    CONV(String, LongLongInt, SyntaxError)
    CONV(String, Float, SyntaxError)
    CONV(String, Double, SyntaxError)
    CONV(String, LongDouble, SyntaxError)
    CONV(String, Time, SyntaxError)

    // Empty string
    CONV(String, Bool, Empty)
    CONV(String, Char, Empty)
    CONV(String, Int, Empty)
    CONV(String, Date, Empty)
    CONV(String, Time, Empty)
    CONV(String, DateTime, Empty)

};

DECLARE_TEST(TestPort)

#endif
