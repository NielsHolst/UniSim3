/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QDate>
#include <QDateTime>
#include <QTime>
#include <base/bare_date.h>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/convert.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "output_file.h"
#include "test_reader_boxscript_x3.h"

#define STR(X) qPrintable(convert<QString>(X))


namespace {
    template <class T>
    auto vstr(QVector<T> v) {
        QStringList list;
        for (auto x : v)
            list << convert<QString>(x);
        return qPrintable(list.join(" "));
    }

    auto str(QString s) {
        return qPrintable(s);
    }
}

void TestReaderBoxScriptX3::testAuxAllTypes() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/aux_all_types.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    // Inherent ports
    try {
        QCOMPARE(root->port("iterations")->value<int>(), 1); // an input
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("iteration" )->value<int>(), 0); // an output
    }
    // Auxillary ports of all possible types of fixed values,
    // because they are fixed they should be transferred already during the construction
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("a")->value().type(), Value::Type::Bool);
        QVERIFY2(root->port("a")->value<bool>(), STR(root->port("a")->value<bool>()));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("b")->value().type(), Value::Type::Int);
        QVERIFY2(root->port("b")->value<int>()==117, STR(root->port("b")->value<int>()));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("c")->value().type(), Value::Type::Double);
        QCOMPARE(root->port("c")->value<double>(), 3.12);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("d")->value().type(), Value::Type::String);
        QCOMPARE(root->port("d")->value<QString>(), QString("abc"));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("e")->value().type(), Value::Type::Date);
        QCOMPARE(root->port("e")->value<QDate>(), QDate(2021,12,24));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("f")->value().type(), Value::Type::Time);
        QCOMPARE(root->port("f")->value<QTime>(), QTime(12,13,14));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("g")->value().type(), Value::Type::DateTime);
        QCOMPARE(root->port("g")->value<QDateTime>(), QDateTime(QDate(2021,12,24), QTime(12,13,14), Qt::UTC));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("h")->value().type(), Value::Type::BareDate);
        QCOMPARE(root->port("h")->value<BareDate>(), BareDate(12,24));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("va")->value().type(), Value::Type::VecBool);
        QVERIFY2(root->port("va")->value<vbool>()==vbool()<<true<<false<<true, vstr(root->port("va")->value<vbool>()));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QVERIFY2(root->port("vb")->value().type()==Value::Type::VecInt, str(root->port("vb")->value().typeName()));
        QVERIFY2(root->port("vb")->value<vint>()==vint()<<133<<-28<<36<<3, vstr(root->port("vb")->value<vint>()));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("vc")->value().type(), Value::Type::VecDouble);
        QCOMPARE(root->port("vc")->value<vdouble>(), vdouble()<<3.12<<34.56);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("vd")->value().type(), Value::Type::VecString);
        QCOMPARE(root->port("vd")->value<vQString>(), vQString()<<"abc"<<"xyz");
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("ve")->value().type(), Value::Type::VecDate);
        QCOMPARE(root->port("ve")->value<vQDate>(), vQDate()<<QDate(2021,12,24)<<QDate(2000,12,31));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("vf")->value().type(), Value::Type::VecTime);
        QCOMPARE(root->port("vf")->value<vQTime>(), vQTime()<<QTime(12,13,14)<<QTime(20,21,22));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("vg")->value().type(), Value::Type::VecDateTime);
        QCOMPARE(root->port("vg")->value<vQDateTime>(), vQDateTime()
                 <<QDateTime(QDate(2021,12,24), QTime(12,13,14), Qt::UTC)
                 <<QDateTime(QDate(2021,12,24), QTime(12,13,15), Qt::UTC));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("vh")->value().type(), Value::Type::VecBareDate);
        QCOMPARE(root->port("vh")->value<vBareDate>(), vBareDate()<<BareDate(12,24)<<BareDate(12,31)<<BareDate(2,29));
    }
    UNEXPECTED_EXCEPTION;
}

void TestReaderBoxScriptX3::testConstructAmend() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/construct_amend.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;
    //
    // Check port values after construction and amend
    //
    try {
        QCOMPARE(root->port("a")->value().type(), Value::Type::Int);
        QCOMPARE(root->port("a")->value<int>(), 10);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("b")->value().type(), Value::Type::Int);
        QCOMPARE(root->port("b")->value<int>(), 20);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("c")->value().type(), Value::Type::Int);
        QCOMPARE(root->port("c")->value<int>(), 20);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("d")->value().type(), Value::Type::Null);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QVERIFY2(root->port("e")->value().type() == Value::Type::Int, qPrintable(root->port("e")->value().typeName()));
        QVERIFY2(root->port("e")->value<int>() == 5, qPrintable(root->port("e")->value().asString()));
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->findOne<Box*>("B")->port("a")->value().type(), Value::Type::Int);
        QCOMPARE(root->findOne<Box*>("B")->port("a")->value<int>(), 20);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->findOne<Port*>("B[b]")->value().type(), Value::Type::Int);
        QCOMPARE(root->findOne<Port*>("B[b]")->value<int>(), -10);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->findOne<Port*>("B[c]")->value().type(), Value::Type::Null);
    }
    UNEXPECTED_EXCEPTION;
}

void TestReaderBoxScriptX3::testInitialize() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/initialize.box"));
        root = Box::root(builder.root());
        root->initializeFamily();
    }
    UNEXPECTED_EXCEPTION;
    //
    // Check port values after initialize
    //
    try {
        QCOMPARE(root->port("a")->value<int>(), 10);
        QCOMPARE(root->port("b")->value<int>(), 20);
        QCOMPARE(root->port("c")->value<int>(), 20);
        QCOMPARE(root->port("d")->value().type(), Value::Type::Null);
        QCOMPARE(root->port("e")->value<int>(), 5);
        QCOMPARE(root->findOne<Port*>("B[a]")->value<int>(), 20);
        QCOMPARE(root->findOne<Port*>("B[b]")->value<int>(), -10);
    }
    UNEXPECTED_EXCEPTION;
}

void TestReaderBoxScriptX3::testInitializePortMissing() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/initialize_port_missing.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        root->initializeFamily();
    }
    UNEXPECTED_EXCEPTION;

    try {
        QCOMPARE(root->findOne<Port*>("B[c]")->value().type(), Value::Type::Null);
    }
    UNEXPECTED_EXCEPTION;
}

void TestReaderBoxScriptX3::testCondition() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/conditional.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        QCOMPARE(root->port("a")->value<int>(),  7);
        QCOMPARE(root->port("b")->value<int>(), 15);
        QCOMPARE(root->port("c")->value<int>(), 19);
    }
    UNEXPECTED_EXCEPTION;
}

void TestReaderBoxScriptX3::testMultipleMatches() {
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/multiple_matches.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        QVERIFY2(root->port("a")->value().type() == Value::Type::Double, str(root->port("a")->value().typeName()));
        QCOMPARE(root->port("a")->value<int>(), 80.);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("b")->value().type(), Value::Type::Int);
        QCOMPARE(root->port("b")->value<int>(), 3);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QVERIFY2(root->port("c")->value().type() == Value::Type::Bool, str(root->port("c")->value().typeName()));
        QCOMPARE(root->port("c")->value<bool>(), true);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QCOMPARE(root->port("k")->value().type(), Value::Type::Null);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QVERIFY2(root->port("u")->value().type() == Value::Type::Int, str(root->port("u")->value().typeName()));
        QCOMPARE(root->port("u")->value<int>(), 20);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QVERIFY2(root->port("v")->value().type() == Value::Type::Int, str(root->port("v")->value().typeName()));
        QCOMPARE(root->port("v")->value<int>(), 0);
    }
    UNEXPECTED_EXCEPTION;
    try {
        QVERIFY2(root->port("w")->value().type() == Value::Type::Bool, str(root->port("c")->value().typeName()));
        QCOMPARE(root->port("w")->value<bool>(), false);
    }
    UNEXPECTED_EXCEPTION;
}

void TestReaderBoxScriptX3::testParentheses() {
    using Type = Value::Type;
    bool excepted(false);
    Box *root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/parentheses.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;

    try {
        QVERIFY2(root->findOne<Port*>("A[a]")->value().type() == Type::Int,
             str(root->findOne<Port*>("A[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("A[a]")->value<int>(), 72);
    }
    UNEXPECTED_EXCEPTION;

    try {
        QVERIFY2(root->findOne<Port*>("A[b]")->value().type() == Type::Double,
             str(root->findOne<Port*>("A[b]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("A[b]")->value<int>(), 80.0);
    }
    UNEXPECTED_EXCEPTION;

    try {
        QVERIFY2(root->findOne<Port*>("A[c]")->value().type() == Type::Double,
             str(root->findOne<Port*>("A[c]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("A[c]")->value<int>(), 6.0);
    }
    UNEXPECTED_EXCEPTION;

}

void TestReaderBoxScriptX3::testDoubleError() {
    bool excepted(false);
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/double_error.box"));
        Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;
}

void TestReaderBoxScriptX3::testOnOffByDate() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/on_off_by_date.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    if (root->port("hasError")->value<bool>()) {
        QFAIL(qPrintable(root->port("errorMsg")->value<QString>()));
    }

    OutputFile f(OutputFile::Format::WithoutFormatLine);
    QVector<int>
        isOn = f.column<int>("time.isOn"),
        expected = {0,0,0,0,1,1,1,1,0,0,0},
        isOn2 = f.column<int>("time2.isOn"),
        expected2  = {1,1,1,0,0,0,0,0,0,0,0};
    QCOMPARE(isOn, expected);
    QCOMPARE(isOn2, expected2);

}

void TestReaderBoxScriptX3::testMissingReference() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/missing_reference.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(root->port("hasError")->value<bool>());
}

void TestReaderBoxScriptX3::testMissingPlotTypes() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/missing_plot_types.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY(!root->port("hasError")->value<bool>());
}
