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
    QVERIFY(port->status() == PortStatus::Redefined);
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
    QVERIFY(port->status() == PortStatus::Redefined);
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
    QVERIFY(port->status() == PortStatus::Redefined);
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
    QVERIFY(port->status() == PortStatus::Redefined);
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
    QVERIFY(port->status() == PortStatus::Redefined);
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
    QVERIFY(port->status() == PortStatus::Redefined);
}

