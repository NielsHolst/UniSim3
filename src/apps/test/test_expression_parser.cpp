/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <string>
#include <base/box.h>
#include <base/boxscript_ast.h>         // defines << operators on AST
#include <base/boxscript_parser.h>      // defines the parser
#include <base/exception.h>
#include <base/expression.h>
#include <base/port.h>
#include "exception_expectation.h"
#include "test_expression_parser.h"

using namespace base;
using boxscript::parser::parse;
using boxscript::parser::Result;

void TestExpressionParser::testAstHandling() {
    bool excepted(false);
    Result ast;
    try {
        ast = parse("Box { &x = 7 + 17.5 }");
    }
    UNEXPECTED_EXCEPTION;

    // Show AST
//    using namespace ast;
//    std::cout << *r << std::endl;

    // Extract expression
    ast::Box root = ast->root;
    QCOMPARE(root.assignments.size(), (size_t) 1);
    ast::Assignment assignment = root.assignments.at(0);
    QCOMPARE(assignment.type(), ast::Assignment::Type::Expression);
    auto expression = boost::get<ast::Expression>(assignment.expression);
    ast::Operand op1 = expression.firstOperand;
    QCOMPARE(expression.operations.size(), (size_t) 1);
    ast::Operation operation = expression.operations.at(0);
    ast::Operand op2 = operation.operand;
    std::string operator_ = operation.operator_;

    QCOMPARE(op1.type(), ast::Operand::Type::Number);
    QCOMPARE(op2.type(), ast::Operand::Type::Number);
    QCOMPARE(operator_, std::string("+"));

    ast::Number num1 = boost::get<ast::Number>(op1),
                           num2 = boost::get<ast::Number>(op2);
    QCOMPARE(num1.type(), ast::Number::Type::Int);
    QCOMPARE(num2.type(), ast::Number::Type::Double);

    int    i = boost::get<int   >(num1);
    double x = boost::get<double>(num2);
    QCOMPARE(i, 7);
    QCOMPARE(x, 17.5);
}

void TestExpressionParser::testOperation() {
    bool excepted(false);
    auto box = std::unique_ptr<Box>( new Box("A", nullptr) );
    auto port = new Port("a", PortType::Auxiliary, box.get());
    Expression e;
    try {
        e = boxscript::parser::parseExpression(port, "7 + 17.5");
    }
    UNEXPECTED_EXCEPTION;
    QCOMPARE(e.stackAsString(true), "7{int} 17.5{double} +{Operator}");
}

void TestExpressionParser::testPathWithPort() {
    bool excepted(false);

    // Parse as full BoxScript
    Result ast;
    try {
        ast = parse("Box { &x = a/b/c[x] }");
    }
    UNEXPECTED_EXCEPTION;

    ast::Box root = ast->root;
    QCOMPARE(root.assignments.size(), (size_t) 1);
    ast::Assignment assignment = root.assignments.at(0);
    QCOMPARE(assignment.type(), ast::Assignment::Type::Expression);
    auto expression = boost::get<ast::Expression>(assignment.expression);
    ast::Operand op1 = expression.firstOperand;
    QCOMPARE(expression.operations.size(), (size_t) 0);
    QCOMPARE(op1.type(), ast::Operand::Type::Path);
    ast::Path astPath = boost::get<ast::Path>(op1);

    // Parse as expression only
    auto box = std::unique_ptr<Box>( new Box("A", nullptr) );
    auto port = new Port("a", PortType::Auxiliary, box.get());
    Expression e;
    try {
        e = boxscript::parser::parseExpression(port, "a/b/c[x]");
    }
    UNEXPECTED_EXCEPTION;

    Expression::Stack elements = e.original();
    QCOMPARE(elements.size(), (uint) 1);
    Expression::Element el = elements[0];
    QCOMPARE(Expression::type(el), Expression::Type::Path);
    Path path = std::get<Path>(el);
    auto alternatives = path.alternatives();
    QCOMPARE(alternatives.size(), 1);
    auto alternative = alternatives.at(0);
    QVERIFY(!alternative.hasRoot());
    auto nodes = alternative.nodes();
    QCOMPARE(nodes.size(), 3);
    QCOMPARE(nodes.at(0).objectName(), QString("a"));
    QCOMPARE(nodes.at(1).objectName(), QString("b"));
    QCOMPARE(nodes.at(2).objectName(), QString("c"));
    auto aport = alternative.port();
    QVERIFY(aport.has_value());
    QCOMPARE(aport.value().name(), QString("x"));
}

void TestExpressionParser::testPathWithoutPort() {
    bool excepted(false);
    auto box = std::unique_ptr<Box>( new Box("A", nullptr) );
    auto port = new Port("a", PortType::Auxiliary, box.get());
    Expression e;
    try {
        e = boxscript::parser::parseExpression(port, "a/b/c[]");
    }
    UNEXPECTED_EXCEPTION;

    Expression::Stack elements = e.original();
    QCOMPARE((int) elements.size(), 1);
    Expression::Element el = elements[0];
    QCOMPARE(Expression::type(el), Expression::Type::Path);
    Path path = std::get<Path>(el);
    auto alternatives = path.alternatives();
    QCOMPARE(alternatives.size(), 1);
    auto alternative = alternatives.at(0);
    QVERIFY(!alternative.hasRoot());
    auto nodes = alternative.nodes();
    QCOMPARE(nodes.size(), 3);
    QCOMPARE(nodes.at(0).objectName(), QString("a"));
    QCOMPARE(nodes.at(1).objectName(), QString("b"));
    QCOMPARE(nodes.at(2).objectName(), QString("c"));
    auto aport = alternative.port();
    QVERIFY(!aport.has_value());
}

void TestExpressionParser::testError() {
    bool excepted(false);
    auto box = std::unique_ptr<Box>( new Box("A", nullptr) );
    auto port = new Port("a", PortType::Auxiliary, box.get());
    Expression e;
    try {
        e = boxscript::parser::parseExpression(port, "*<PageR[xAxis]>");
    }
    EXPECTED_EXCEPTION_SHOWN;
}
