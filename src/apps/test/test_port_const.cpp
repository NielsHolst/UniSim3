#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include <base/port.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "test_port_const.h"

void TestPortConst::testAllDefault() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("port_const/all_default.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    Port *port;
    // A construction value default is constant
    port = root->findOne<Port*>("sim[iterations]");
    QVERIFY(port->isConstant());

    // A construction expression default is constant, if it relies only on non-existing ports
    port = root->findOne<Port*>("sim[steps]");
    QVERIFY(port->isConstant());

    // An output port is not constant
    port = root->findOne<Port*>("sim[step]");
    QVERIFY(!port->isConstant());

    // A type default is constant
    port = root->findOne<Port*>("sum[values]");
    QVERIFY(port->isConstant());
}

void TestPortConst::testAux() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("port_const/aux_port.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    Port
        *a    = root->findOne<Port*>("box[a]"),
        *b    = root->findOne<Port*>("box[b]"),
        *simb = root->findOne<Port*>("sim[b]"),
        *c    = root->findOne<Port*>("box[c]");

    // box[b] is not constant because the constness of box[x] is updated before the constness of its import sim[steps]
    QVERIFY(   a->isConstant());
    QVERIFY(  !b->isConstant());
    QVERIFY(simb->isConstant());
    QVERIFY(  !c->isConstant());
}

void TestPortConst::testExprRedefinesConst() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("port_const/expr_redefines_const.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;

    QVERIFY( root->findOne<Port*>("a[latitude]")->isConstant());
    QVERIFY(!root->findOne<Port*>("b[latitude]")->isConstant());
    QVERIFY(!root->findOne<Port*>("a[dayOfYear]")->isConstant());
    QVERIFY( root->findOne<Port*>("b[dayOfYear]")->isConstant());
    QVERIFY( root->findOne<Port*>("x[values]")->isConstant());
    QVERIFY(!root->findOne<Port*>("y[values]")->isConstant());
}

