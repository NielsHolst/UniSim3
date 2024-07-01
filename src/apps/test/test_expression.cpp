/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <boost/variant/get.hpp>
#include <QVector>
#include <base/box.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/expression.h>
#include <base/port.h>
#include <base/operator.h>
#include <base/test_num.h>
#include "exception_expectation.h"
#include "test_expression.h"

using boost::get;
using namespace base;
using namespace TestNum;

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

    QCOMPARE(expr.originalAsString(true).replace(" ", ""), "17.5{double}+{Operator}123{int}*{Operator}2{int}");
    try {
        expr.close();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(expr.stackAsString(true).replace(" ", ""), "17.5{double}123{int}2{int}*{Operator}+{Operator}");

    expr.clear();
    expr.push(17.5);
    expr.push(Operator::Multiply);
    expr.push(123);
    expr.push(Operator::Add);
    expr.push(2);

    QCOMPARE(expr.originalAsString(true).replace(" ", ""), "17.5{double}*{Operator}123{int}+{Operator}2{int}");
    try {
        expr.close();
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(expr.stackAsString(true).replace(" ", ""), "17.5{double}123{int}*{Operator}2{int}+{Operator}");



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
    QCOMPARE(e.stackAsString(true), "8{int} 9{int} 2{int} *{Operator} +{Operator}");

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
    QCOMPARE(e.stackAsString(true), "8{int} +{Operator} 9.5{double} *{Operator} 2{int}");
    e.close();
    QCOMPARE(e.stackAsString(true), "8{int} 9.5{double} 2{int} *{Operator} +{Operator}");

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
    QCOMPARE(e.stackAsString(true), "8{int} -{Operator} 9{int} 2{int} *{Operator} +{Operator}");

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
    QCOMPARE(e.stackAsString(true), "8{int} 9{int} 2{int} ^{Operator} 4{int} *{Operator} +{Operator}");

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
    QCOMPARE(e.stackAsString(true).replace(" ", ""),
        "8{int}2{int}3{int}*{Operator}+{Operator},{Operator}"
        "13{int},{Operator}14{int},{Operator}sum[3]({FunctionCall}100{int}-{Operator}");
    // Notice unmatched left parenthesis           ^ in expectation
    // Might be due to the handcrafted e or a minor error in stackAsString

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
    QCOMPARE(e.stackAsString(true),
        "3{int} 4{int} +{Operator}");

    Value result = e.evaluate();
    QCOMPARE(result.type(), Value::Type::Int);
    QCOMPARE(result.as<int>(), 7);
}

namespace {

bool eq(QVector<double> a, QVector<double> b, double eps = 1e-3) {
    if (a.size() != b.size()) {
        std::cerr << "Vectors of different sizes: "  << a.size() << " " << b.size() << std::endl;
        return false;
    }
    for (int i = 0; i < a.size(); ++i) {
        double x = a.at(i),
               y = b.at(i);
        if (!TestNum::eq(x, y, eps)) {
            std::cerr << "Values differ at position " << i  << ": " << x << " vs. " << y << std::endl;
            return false;
        }
    }
    return true;
}

}

void TestExpression::textBoxScriptExpression() {
    using VecInt = QVector<int>;
    using VecDouble = QVector<double>;
    using VecString = QVector<QString>;

    bool excepted(false);
    try {
        Command::submit(QStringList() << "run" << "expressions/expressions.box");
    }
    UNEXPECTED_EXCEPTION;

    QVERIFY(Box::hasRoot());
    Box *root = Box::root();

    try {
        QVERIFY(root->findOne<Port*>("equations[aboveThreshold]")->value<bool>());
        QVERIFY(eq(root->findOne<Port*>("equations[tempInside]")->value<double>(), 23.0));
        QVERIFY(eq(root->findOne<Port*>("equations[biomass]")->value<double>(), 2.306e6, 1e-3));
        QVERIFY(eq(root->findOne<Port*>("equations[homeArea]")->value<double>(), 23.76, 1e-3));
        QVERIFY(eq(root->findOne<Port*>("equations[supplyDemandRatio]")->value<double>(), 0.667, 1e-3));
        QVERIFY(eq(root->findOne<Port*>("equations[demand]")->value<double>(), 7.6, 1e-3));

        QCOMPARE(root->findOne<Port*>("vectors[scalars]")->value<int>(), 12);
        QCOMPARE(root->findOne<Port*>("vectors[mixed]")->value<VecInt>(), VecInt() << 10 << 12 << 16);
        QCOMPARE(root->findOne<Port*>("vectors[vectors]")->value<VecInt>(), VecInt() << 5 << -2 << 21);
        QCOMPARE(root->findOne<Port*>("vectors[square]")->value<VecInt>(), VecInt() << 4 << 9 );
        QCOMPARE(root->findOne<Port*>("vectors[a_squared]")->value<VecInt>(), VecInt() << 49 << 81 << 169);

        QCOMPARE(root->findOne<Port*>("conditions[d]")->value<int>(), 7);
        QCOMPARE(root->findOne<Port*>("conditions[e]")->value<int>(), -1);

        QCOMPARE(root->findOne<Port*>("functions/sum[x]")->value<int>(), 23);
        QCOMPARE(root->findOne<Port*>("functions/sum[y]")->value<int>(),  0);
        QCOMPARE(root->findOne<Port*>("functions/sum[z]")->value<int>(), 30);
        QCOMPARE(root->findOne<Port*>("functions/sum[w]")->value<int>(), 53);

        QVERIFY(eq(root->findOne<Port*>("functions/mean[x]")->value<double>(), 7.67, 1e-3));
        QVERIFY(eqZero(root->findOne<Port*>("functions/mean[y]")->value<double>()));
        QVERIFY(eq(root->findOne<Port*>("functions/mean[z]")->value<double>(), 15.0, 1e-3));
        QVERIFY(eq(root->findOne<Port*>("functions/mean[w]")->value<double>(), 10.6, 1e-3));

        QCOMPARE(root->findOne<Port*>("functions/count[x]")->value<int>(), 3);
        QCOMPARE(root->findOne<Port*>("functions/count[y]")->value<int>(), 0);
        QCOMPARE(root->findOne<Port*>("functions/count[z]")->value<int>(), 2);
        QCOMPARE(root->findOne<Port*>("functions/count[w]")->value<int>(), 5);

        QCOMPARE(root->findOne<Port*>("functions/exists[a]")->value<bool>(), true);
        QCOMPARE(root->findOne<Port*>("functions/exists[b]")->value<bool>(), true);
        QCOMPARE(root->findOne<Port*>("functions/exists[c]")->value<bool>(), false);
        QCOMPARE(root->findOne<Port*>("functions/exists[d]")->value<bool>(), false);
        QCOMPARE(root->findOne<Port*>("functions/exists[e]")->value<bool>(), false);

//        Port *b = root->findOne<Port*>("functions/math[b]"); // PROBLEM: port's value has type QVector<double>, should be double
        QVERIFY(eq(root->findOne<Port*>("functions/math[b]")->value<VecDouble>(), VecDouble() << 2.0));
        QVERIFY(eq(root->findOne<Port*>("functions/math[d]")->value<VecDouble>(), VecDouble() << 4.60517 << 0.693147));
        QVERIFY(eq(root->findOne<Port*>("functions/math[e]")->value<VecDouble>(), VecDouble() << 0.0 << 2.0 << 1.0));
        // Same PROBLEM: f and g should be double
        QVERIFY(eq(root->findOne<Port*>("functions/math[f]")->value<VecDouble>(), VecDouble() << -5.0));
        QVERIFY(eq(root->findOne<Port*>("functions/math[g]")->value<VecDouble>(), VecDouble() << 100.0));

        QVERIFY(!root->findOne<Port*>("functions/logical[c]")->value<bool>());
        QVERIFY( root->findOne<Port*>("functions/logical[d]")->value<bool>());
        QVERIFY( root->findOne<Port*>("functions/logical[e]")->value<bool>());
        QVERIFY(!root->findOne<Port*>("functions/logical[f]")->value<bool>());
        QVERIFY( root->findOne<Port*>("functions/logical[g]")->value<bool>());

        QCOMPARE(root->findOne<Port*>("functions/combine[d]")->value<VecDouble>(), VecDouble() << 5.6 << -3.7);
        QCOMPARE(root->findOne<Port*>("functions/combine[e]")->value<VecString>(), VecString() << "5" << "7" << "message");
        QCOMPARE(root->findOne<Port*>("functions/combine[f]")->value<VecDouble>(), VecDouble() << 5.0 << 7.0 << 2.1);
        QCOMPARE(root->findOne<Port*>("functions/combine[g]")->value<VecString>(), VecString() << "5.6" << "-3.7" << "5" << "7" << "message");

        QCOMPARE(root->findOne<Port*>("functions/names[a]")->value<QString>(), "A");
        QCOMPARE(root->findOne<Port*>("functions/names[b]")->value<QString>(), "B");
        QCOMPARE(root->findOne<Port*>("functions/names[c]")->value<VecString>(), VecString() << "B" << "A");


    }
    UNEXPECTED_EXCEPTION;
}
