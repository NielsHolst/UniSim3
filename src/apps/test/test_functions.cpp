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
#include <base/resolved_references.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "test_functions.h"

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

void TestFunctions::test_c() {
    using Type = Value::Type;
    bool excepted(false);
    std::unique_ptr<Box> root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/function_c.box"));
        root = std::unique_ptr<Box>( builder.root() );
    }
    UNEXPECTED_EXCEPTION;

    try {
        QVERIFY2(root->findOne<Port*>("Null[a]")->value().type() == Type::Null,
             str(root->findOne<Port*>("Null[a]")->value().typeName()));

        QVERIFY2(root->findOne<Port*>("Null[b]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("Null[b]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Null[b]")->value<vint>(), vint()<<7);

        QVERIFY2(root->findOne<Port*>("Null[c]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("Null[c]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Null[c]")->value<vint>(), vint()<<10<<7);

        QVERIFY2(root->findOne<Port*>("Null[d]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("Null[d]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Null[d]")->value<vint>(), vint()<<11<<7);

        QVERIFY2(root->findOne<Port*>("Length[a]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("Length[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Length[a]")->value<vint>(), vint()<<117);

        QVERIFY2(root->findOne<Port*>("Length[b]")->value().type() == Type::VecBool,
             str(root->findOne<Port*>("Length[b]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Length[b]")->value<vbool>(), vbool()<<false);

        QVERIFY2(root->findOne<Port*>("Mixed[a]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("Mixed[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Mixed[a]")->value<vint>(), vint()<<6<<7<<8);

        QVERIFY2(root->findOne<Port*>("Mixed[b]")->value().type() == Type::VecDouble,
             str(root->findOne<Port*>("Mixed[b]")->value().typeName()));
        QVERIFY2(root->findOne<Port*>("Mixed[b]")->value<vdouble>() == vdouble()<<7.0<<9.5<<-9.0,
            vstr(root->findOne<Port*>("Mixed[b]")->value<vdouble>()));

        QVERIFY2(root->findOne<Port*>("Mixed[c]")->value().type() == Type::VecDouble,
             str(root->findOne<Port*>("Mixed[c]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Mixed[c]")->value<vdouble>(), vdouble()<<7.3<<5e-6);

        QVERIFY2(root->findOne<Port*>("Mixed[d]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("Mixed[d]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Mixed[d]")->value<vint>(), vint()<<5<<1);

        QVERIFY2(root->findOne<Port*>("Mixed[e]")->value().type() == Type::VecDouble,
             str(root->findOne<Port*>("Mixed[e]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Mixed[e]")->value<vdouble>(), vdouble()<<5.0<<1.0);

        QVERIFY2(root->findOne<Port*>("Mixed[f]")->value().type() == Type::VecString,
             str(root->findOne<Port*>("Mixed[f]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Mixed[f]")->value<vQString>(), vQString()<<"dog"<<"cat");

        QVERIFY2(root->findOne<Port*>("Mixed[g]")->value().type() == Type::VecString,
             str(root->findOne<Port*>("Mixed[g]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Mixed[g]")->value<vQString>(), vQString()<<"dog"<<"cat"<<"7");

        QVERIFY2(root->findOne<Port*>("Mixed[h]")->value().type() == Type::VecString,
             str(root->findOne<Port*>("Mixed[h]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Mixed[h]")->value<vQString>(), vQString()<<"dog"<<"cat"<<"1");

        QVERIFY2(root->findOne<Port*>("Nested[a]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("Nested[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Nested[a]")->value<vint>(), vint()<<6<<7<<8<<1<<2);

        QVERIFY2(root->findOne<Port*>("Nested[b]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("Nested[b]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Nested[b]")->value<vint>(), vint()<<6<<7<<8<<1<<2);

        QVERIFY2(root->findOne<Port*>("Nested[c]")->value().type() == Type::VecDouble,
             str(root->findOne<Port*>("Nested[c]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Nested[c]")->value<vdouble>(), vdouble()<<6<<7<<8<<1.5<<2);

        QVERIFY2(root->findOne<Port*>("Expr[a]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("Expr[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Expr[a]")->value<vint>(), vint()<<6<<13<<-8<<3);

        QVERIFY2(root->findOne<Port*>("Expr[b]")->value().type() == Type::VecDouble,
             str(root->findOne<Port*>("Expr[b]")->value().typeName()));
        QVERIFY2(root->findOne<Port*>("Expr[b]")->value<vdouble>() == vdouble()<<6<<13<<-8<<1.5,
            vstr(root->findOne<Port*>("Expr[b]")->value<vdouble>()));

        QVERIFY2(root->findOne<Port*>("Expr[c]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("Expr[c]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("Expr[c]")->value<vint>(), vint()<<6<<-8<<3);

        QVERIFY2(root->findOne<Port*>("A[a]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("A[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("A[a]")->value<vint>(), vint()<<10<<8<<605);

        QVERIFY2(root->findOne<Port*>("E[a]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("E[a]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("E[a]")->value<vint>(), vint()<<10<<8<<605);

        QVERIFY2(root->findOne<Port*>("E[b]")->value().type() == Type::VecInt,
             str(root->findOne<Port*>("E[b]")->value().typeName()));
        QCOMPARE(root->findOne<Port*>("E[b]")->value<vint>(), vint()<<10<<8);

    }
    UNEXPECTED_EXCEPTION;
}

void TestFunctions::test_name() {
    bool excepted(false);
    Box* root;
    BoxBuilder builder;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/function_names.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;

//    std::cout << STR(root->toText("ioa")) << std::endl;

    Port *port;
    Expression expr;
    Expression::Stack stack;
    try {
        port = root->findMaybeOne<Port*>("E[childrenOfA]");
        QVERIFY(port);
        expr = port->expression();
        stack = expr.stack();

        QCOMPARE(expr.size(), 1);
        QCOMPARE(stack.size(), size_t(1));
        QVERIFY2(Expression::type(stack.at(0)) == Expression::Type::BoxPtrs, STR(Expression::typeName(stack.at(0))));

        // QVERIFY(ResolvedReferences::fixed()); Functionality not used?

        QCOMPARE(
            root->findOne<Port*>("E[childrenOfANames]")->value<QVector<QString>>(),
            QVector<QString>() << "B" << "C" << "D"
        );
        QCOMPARE(
            root->findOne<Port*>("E[childBName]")->value<QVector<QString>>(),
            QVector<QString>() << "B"
        );
        QCOMPARE(root->findOne<Port*>("E[childBName]")->value<QString>(), QString("B"));
    }
    UNEXPECTED_EXCEPTION;
}

