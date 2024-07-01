/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include <base/port.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "test_port_status.h"

void TestPortStatus::testAllDefault() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("port_status/all_default.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    Port *port = root->findOne<Port*>("sim[steps]");
    QVERIFY(port->status() == PortStatus::ConstructionDefault);

    port = root->findOne<Port*>("sim[step]");
    QVERIFY(port->status() == PortStatus::TypeDefault);

    port = root->findOne<Port*>("sim[iterations]");
    QVERIFY(port->status() == PortStatus::ConstructionDefault);

    port = root->findOne<Port*>("sum[values]");
    QVERIFY(port->status() == PortStatus::TypeDefault);
}

void TestPortStatus::testAuxDefinesExpr() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("port_status/aux_defines_expr.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    Port *port = root->findOne<Port*>("a[new]");
    QVERIFY(port->status() == PortStatus::UserDefined);
}

void TestPortStatus::testAuxDefinesValue() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("port_status/aux_defines_value.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    Port *port = root->findOne<Port*>("a[new]");
    QVERIFY(port->status() == PortStatus::UserDefined);
}

void TestPortStatus::testExprRedefinesExpr() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("port_status/expr_redefines_expr.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    Port *port = root->findOne<Port*>("sim[steps]");
    QVERIFY(port->status() == PortStatus::UserDefined);
}

void TestPortStatus::testExprRedefinesValue() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("port_status/expr_redefines_value.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    Port *port = root->findOne<Port*>("sim[iterations]");
    QVERIFY(port->status() == PortStatus::UserDefined);
}

void TestPortStatus::testValueRedefinesExpr() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("port_status/value_redefines_expr.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    Port *port = root->findOne<Port*>("sim[steps]");
    QVERIFY(port->status() == PortStatus::UserDefined);
}

void TestPortStatus::testValueRedefinesValue() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("port_status/value_redefines_value.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;
    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    Port *port = root->findOne<Port*>("sim[iterations]");
    QVERIFY(port->status() == PortStatus::UserDefined);
}

void TestPortStatus::testValueRedefinesDefault() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("port_status/value_redefines_default.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;
    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    // A[ints] is type default
    // A[strings] is construction default

    // Box a redefines neither; a[strings] matches two ports
    // Box b redefines both
    // Box c redefines neither, c[strings] matches no ports
    Port
        *a_ints    = root->findOne<Port*>("a[ints]"),
        *a_strings = root->findOne<Port*>("a[strings]"),
        *b_ints    = root->findOne<Port*>("b[ints]"),
        *b_strings = root->findOne<Port*>("b[strings]"),
        *c_strings = root->findOne<Port*>("c[strings]");

    QCOMPARE(a_ints->status(), PortStatus::TypeDefault);
    QCOMPARE(a_strings->status(), PortStatus::ConstructionDefault);
    QCOMPARE(b_ints->status(), PortStatus::UserDefined);
    QCOMPARE(b_strings->status(), PortStatus::UserDefined);

    // Test values as well
    QVector<int>
        a_ints_expected,
        b_ints_expected = {13,7};
    QVector<QString>
        ab_strings_expected = {"abc", "xyz"},
        c_strings_expected;

    QCOMPARE(a_ints->value<QVector<int>>(), a_ints_expected);
    QCOMPARE(b_ints->value<QVector<int>>(), b_ints_expected);
    QCOMPARE(a_strings->value<QVector<QString>>(), ab_strings_expected);
    QCOMPARE(b_strings->value<QVector<QString>>(), ab_strings_expected);
    QCOMPARE(b_strings->value<QVector<QString>>(), ab_strings_expected);
    QCOMPARE(c_strings->value<QVector<QString>>(), c_strings_expected);
}
