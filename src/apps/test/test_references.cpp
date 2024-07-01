/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/convert.h>
#include <base/exception.h>
#include <base/expression.h>
#include <base/reader_boxscript.h>
#include <base/resolved_references.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "test_references.h"

#define STR(X) qPrintable(convert<QString>(X))

namespace {
    auto str(QString s) {
        return qPrintable(s);
    }
}

void TestReferences::testForward() {
    using Type = Value::Type;
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/reference_forward.box"));
        root = Box::root(builder.root());
        root->initializeFamily();
    }
    UNEXPECTED_EXCEPTION;

    try {
        QVERIFY2(root->findOne<Port*>("A[a]")->value().type() == Type::Date,
             str(root->findOne<Port*>("A[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("A[a]")->value<QDate>(), QDate(2021,12,24));

        QVERIFY2(root->findOne<Port*>("B[a]")->value().type() == Type::Date,
             str(root->findOne<Port*>("B[a]")->value().typeName()));

        QVERIFY2(root->findOne<Port*>("B[b]")->value().type() == Type::Date,
             str(root->findOne<Port*>("B[b]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("B[b]")->value<QDate>(), QDate(2021,12,24));

        QVERIFY2(root->findOne<Port*>("C[a]")->value().type() == Type::Null,
             str(root->findOne<Port*>("C[a]")->value().typeName()));

        QVERIFY2(root->findOne<Port*>("C[b]")->value().type() == Type::Date,
             str(root->findOne<Port*>("C[b]")->value().typeName()));

        QVERIFY2(root->findOne<Port*>("C[c]")->value().type() == Type::Date,
             str(root->findOne<Port*>("C[c]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("C[c]")->value<QDate>(), QDate(2021,12,24));

        QVERIFY2(root->findOne<Port*>("D[a]")->value().type() == Type::Double,
             str(root->findOne<Port*>("D[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("D[a]")->value<double>(), 13.1412);

        QVERIFY2(root->findOne<Port*>("E[a]")->value().type() == Type::Double,
             str(root->findOne<Port*>("E[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("E[a]")->value<double>(), 23.1412);

        QVERIFY2(root->findOne<Port*>("E[b]")->value().type() == Type::Double,
             str(root->findOne<Port*>("E[b]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("E[b]")->value<double>(), 13.1412);

        QVERIFY2(root->findOne<Port*>("F[a]")->value().type() == Type::Int,
             str(root->findOne<Port*>("F[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("F[a]")->value<int>(), 30);

        QVERIFY2(root->findOne<Port*>("F[b]")->value().type() == Type::Double,
             str(root->findOne<Port*>("F[b]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("F[b]")->value<double>(), 23.1412);

        QVERIFY2(root->findOne<Port*>("F[c]")->value().type() == Type::Double,
             str(root->findOne<Port*>("F[c]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("F[c]")->value<double>(), 13.1412);
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->resetFamily();
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->updateFamily();
        root->updateFamily();
        root->updateFamily();
    }
    UNEXPECTED_EXCEPTION;
}

void TestReferences::testUpdate() {
    using Type = Value::Type;
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/reference_update.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        QVERIFY2(root->findOne<Port*>("A[aaa]")->value().type() == Type::Uninitialized,
             str(root->findOne<Port*>("A[aaa]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("A[aaa]")->value<int>(), 0);
    }
    UNEXPECTED_EXCEPTION;

    root->run();
    QCOMPARE(root->findOne<Port*>("A[aaa]")->value<int>(), 140);
    QCOMPARE(root->findOne<Port*>("A[b]"  )->value<int>(), 140);
    QCOMPARE(root->findOne<Port*>("A[c]"  )->value<int>(), 140);
    QCOMPARE(root->findOne<Port*>("A[d]"  )->value<int>(), 140);

    std::cout
            << root->findOne<Port*>("B[d]"  )->value<int>() << std::endl
            << root->findOne<Port*>("B[c]"  )->value<int>() << std::endl
            << root->findOne<Port*>("B[b]"  )->value<int>() << std::endl
            << root->findOne<Port*>("B[a]"  )->value<int>() << std::endl;

    QCOMPARE(root->findOne<Port*>("B[d]"  )->value<int>(), 140);
    QCOMPARE(root->findOne<Port*>("B[c]"  )->value<int>(), 140);
    QCOMPARE(root->findOne<Port*>("B[b]"  )->value<int>(), 140);
    QCOMPARE(root->findOne<Port*>("B[a]"  )->value<int>(), 120);
}

void TestReferences::testDots() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/reference_dots.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    QCOMPARE(root->findOne<Port*>("A[a]")->value<double>(), 3.12);
    QCOMPARE(root->findOne<Port*>("A[b]")->value<double>(), 3.12);
    QCOMPARE(root->findOne<Port*>("B[c]")->value<double>(), 3.12);

}


void TestReferences::testEmptyVector() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/empty_vector.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    QVector<QString> expected = {"abc", "xyz"};
    QCOMPARE(root->findOne<Port*>("a[strings]")->value<QVector<QString>>(), expected);
    QVector<int> expectedInt = {13,7};
    QCOMPARE(root->findOne<Port*>("b[ints]")->value<QVector<int>>(), expectedInt);
    QCOMPARE(root->findOne<Port*>("b[strings]")->value<QVector<QString>>(), QVector<QString>());
    QCOMPARE(root->findOne<Port*>("c[ints]")->value<QVector<int>>(), QVector<int>());
}
