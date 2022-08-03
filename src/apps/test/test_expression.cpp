#include <iostream>
#include <boost/variant/get.hpp>
#include <base/box.h>
#include <base/port.h>
#include <base/expression.h>
#include <base/operator.h>
#include "exception_expectation.h"
#include "test_expression.h"

using boost::get;
using namespace base;

void TestExpression::testPush() {
    Expression expr;
    expr.push(17.5);
    expr.push(123);
    expr.push(2);
    expr.push(Operator::Multiply);
    expr.push(Operator::Add);

    QCOMPARE(Expression::type(expr.at(0)), Expression::Type::Value);
    QCOMPARE(Expression::type(expr.at(1)), Expression::Type::Value);
    QCOMPARE(Expression::type(expr.at(2)), Expression::Type::Value);
    QCOMPARE(Expression::type(expr.at(3)), Expression::Type::Operator);
    QCOMPARE(Expression::type(expr.at(4)), Expression::Type::Operator);
}

void TestExpression::testToPostfix() {
    bool excepted(false);
    Expression expr;
    expr.push(17.5);
    expr.push(Operator::Add);
    expr.push(123);
    expr.push(Operator::Multiply);
    expr.push(2);

    QCOMPARE(expr.originalAsString(), "17.5{double} +{Operator} 123{int} *{Operator} 2{int}");
    try {
        expr.close();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(expr.stackAsString(), "17.5{double} 123{int} 2{int} *{Operator} +{Operator}");

    expr.clear();
    expr.push(17.5);
    expr.push(Operator::Multiply);
    expr.push(123);
    expr.push(Operator::Add);
    expr.push(2);

    QCOMPARE(expr.originalAsString(), "17.5{double} *{Operator} 123{int} +{Operator} 2{int}");
    try {
        expr.close();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(expr.stackAsString(), "17.5{double} 123{int} *{Operator} 2{int} +{Operator}");



}

void TestExpression::testEmpty() {
    bool excepted(false);
    Expression expr;
    try {
        expr.close();
    }
    UNEXPECTED_EXCEPTION;

    try {
        expr.evaluate();
    }
    EXPECTED_EXCEPTION_SHOWN;

    QVERIFY(expr.isEmpty());
}

void TestExpression::testSingle() {
    bool excepted(false);
    auto box = std::unique_ptr<Box>( new Box("A", nullptr) );
    auto port = new Port("a", PortType::Auxiliary, box.get());
    Expression expr(port);
    Value value;
    expr.push("abc");
    try {
        expr.close();
        value = expr.evaluate();
    }
    UNEXPECTED_EXCEPTION;

    QCOMPARE(value.as<QString>(), QString("abc"));
}

void TestExpression::testAddition() {
    bool excepted(false);
    using E    = Expression;
    using Type = Value::Type;
    auto box = std::unique_ptr<Box>( new Box("A", nullptr) );
    auto port = new Port("a", PortType::Auxiliary, box.get());
    E e(port);

    e.push(8);
    e.push(Operator::Add);
    e.push(9);
    e.push(Operator::Multiply);
    e.push(2);
    e.close();
    QCOMPARE(e.stackAsString(), "8{int} 9{int} 2{int} *{Operator} +{Operator}");

    Value result;
    try {
        result = e.evaluate();
        result.as<int>();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result.type(), Type::Int);
    QCOMPARE(result.as<int>(), 26);
}

void TestExpression::testResultType() {
    bool excepted(false);
    using E    = Expression;
    using Type = Value::Type;
    auto box = std::unique_ptr<Box>( new Box("A", nullptr) );
    auto port = new Port("a", PortType::Auxiliary, box.get());
    E e(port);

    e.push(8);
    e.push(Operator::Add);
    e.push(9.5);
    e.push(Operator::Multiply);
    e.push(2);
    QCOMPARE(e.stackAsString(), "8{int} +{Operator} 9.5{double} *{Operator} 2{int}");
    e.close();
    QCOMPARE(e.stackAsString(), "8{int} 9.5{double} 2{int} *{Operator} +{Operator}");

    Value result;
    try {
        result = e.evaluate();
        result.as<double>();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result.type(), Type::Double);
    QCOMPARE(result.as<double>(), 27.);
}

void TestExpression::testNegation() {
    bool excepted(false);
    using E    = Expression;
    using Type = Value::Type;
    auto box = std::unique_ptr<Box>( new Box("A", nullptr) );
    auto port = new Port("a", PortType::Auxiliary, box.get());
    E e(port);

    e.push(Operator::Negate);
    e.push(8);
    e.push(Operator::Add);
    e.push(9);
    e.push(Operator::Multiply);
    e.push(2);
    e.close();
    QCOMPARE(e.stackAsString(), "8{int} --{Operator} 9{int} 2{int} *{Operator} +{Operator}");

    Value result;
    try {
        result = e.evaluate();
        result.as<int>();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result.type(), Type::Int);
    QCOMPARE(result.as<int>(), 10);
}

void TestExpression::testExponentiation() {
    bool excepted(false);
    using E    = Expression;
    using Type = Value::Type;
    auto box = std::unique_ptr<Box>( new Box("A", nullptr) );
    auto port = new Port("a", PortType::Auxiliary, box.get());
    E e(port);

    e.push(8);
    e.push(Operator::Add);
    e.push(9);
    e.push(Operator::Exponentiate);
    e.push(2);
    e.push(Operator::Multiply);
    e.push(4);
    e.close();
    QCOMPARE(e.stackAsString(), "8{int} 9{int} 2{int} ^{Operator} 4{int} *{Operator} +{Operator}");

    Value result;
    try {
        result = e.evaluate();
        result.as<int>();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result.type(), Type::Int);
    QCOMPARE(result.as<int>(), 8 + 9*9*4);
}

void TestExpression::testFunctionCall() {
    bool excepted(false);
    using E     = Expression;
    using Func  = Expression::FunctionCall;
    auto box = std::unique_ptr<Box>( new Box("A", nullptr) );
    auto port = new Port("a", PortType::Auxiliary, box.get());
    E e(port);

    e.push(Func("sum", 3));
    e.push(8);
    e.push(Operator::Add);
    e.push(2);
    e.push(Operator::Multiply);
    e.push(3);
    e.push(Operator::Comma);
    e.push(13);
    e.push(Operator::Comma);
    e.push(14);
    e.push(Operator::Comma);
    e.push(Expression::FunctionCallEnd());
    e.push(Operator::Subtract);
    e.push(100);
    try {
        e.close();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(e.stackAsString(),
        "8{int} 2{int} 3{int} *{Operator} +{Operator} ,{Operator} "
        "13{int} ,{Operator} 14{int} ,{Operator} sum[3]{FunctionCall} 100{int} -{Operator}");

    Value result;
    try {
        result = e.evaluate();
        result.as<int>();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(result.type(), Value::Type::Int);
    QCOMPARE(result.as<int>(), 8 + 2 * 3 + 13 +14 - 100);
}

void TestExpression::testParentheses() {
    bool excepted(false);
    using E     = Expression;
    auto box = std::unique_ptr<Box>( new Box("A", nullptr) );
    auto port = new Port("a", PortType::Auxiliary, box.get());
    E e(port);

    e.push(Parenthesis::Left);
    e.push(3);
    e.push(Operator::Add);
    e.push(4);
    e.push(Parenthesis::Right);

    try {
        e.close();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(e.stackAsString(),
        "3{int} 4{int} +{Operator}");

    Value result = e.evaluate();
    QCOMPARE(result.type(), Value::Type::Int);
    QCOMPARE(result.as<int>(), 7);


}
