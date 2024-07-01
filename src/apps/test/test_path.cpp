/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/box.h>
#include <base/box_builder.h>
#include <base/computation.h>
#include <base/path.h>
#include <base/port.h>
#include "exception_expectation.h"
#include "test_path.h"

using namespace base;

void TestPath::testFindBox() {
    bool excepted(false);
    Path path;
    try {
        path = Path("Calendar::calendar");
    }
    UNEXPECTED_EXCEPTION;

    auto alternatives = path.alternatives();
    QCOMPARE(alternatives.size(), 1);
    auto alternative = alternatives.at(0);
    QCOMPARE(alternative.nodes().size(), 1);
    auto node = alternative.nodes().at(0);
    QCOMPARE(node.directive(), Path::Directive::Any);
    QCOMPARE(node.className(), QString("Calendar"));
    QCOMPARE(node.objectName(), QString("calendar"));
}


void TestPath::testFindClassName() {
    bool excepted(false);
    using Boxes = QVector<Box*>;
    using Ports = QVector<Port*>;
    Computation::changeStep(Computation::Step::Construct);

    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("S").
                box().name("A").endbox().
                box("Stage").name("B").endbox().
            endbox();
    }
    UNEXPECTED_EXCEPTION;

    auto root = std::unique_ptr<Box>( builder.root() );
    try {
        root->initializeFamily();
    }
    UNEXPECTED_EXCEPTION;

    Box *A, *S, *B;
    Port *P;
    try {
        S = root->findOne<Box*>("S");
        A = root->findOne<Box*>("A");
        B = root->findOne<Box*>("B");
        P = root->findOne<Port*>("B[k]");

        QCOMPARE(S->name(), "S");
        QCOMPARE(A->name(), "A");
        QCOMPARE(B->name(), "B");

        QCOMPARE(root->findMany<Box*>("Box::S"), Boxes() << S);
        QCOMPARE(root->findMany<Box*>("Box::A"), Boxes() << A);
        QCOMPARE(root->findMany<Box*>("Box::B"), Boxes() << B);
        QCOMPARE(root->findMany<Box*>("Box::*"), Boxes() << A << B << S);

        QCOMPARE(root->findMany<Box*>("Stage::S"), Boxes());
        QCOMPARE(root->findMany<Box*>("Stage::A"), Boxes());
        QCOMPARE(root->findMany<Box*>("Stage::B"), Boxes() << B);
        QCOMPARE(root->findMany<Box*>("Stage::*"), Boxes() << B);

        QCOMPARE(root->findMany<Port*>("Box::B[k]"  ), Ports() << P);
        QCOMPARE(root->findMany<Port*>("Stage::B[k]"), Ports() << P);
        QCOMPARE(root->findMany<Port*>("Box::*[k]"  ), Ports() << P);
        QCOMPARE(root->findMany<Port*>("Stage::*[k]"), Ports() << P);
    }
    UNEXPECTED_EXCEPTION;
}

void TestPath::testLookInChildrenNotSelf() {
    bool excepted(false);
    using Ports = QVector<Port*>;

    Computation::changeStep(Computation::Step::Construct);
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("S").
                box().name("A").
                    aux("x").equals(10).
                    box().name("B").
                        aux("x").equals(20).
                    endbox().
                    box().name("B").
                        aux("x").equals(30).
                    endbox().
                endbox().
            endbox();
    }
    UNEXPECTED_EXCEPTION;

    auto root = std::unique_ptr<Box>( builder.root() );
    try {
        root->initializeFamily();
    }
    UNEXPECTED_EXCEPTION;

    Box *A = root->findOne<Box*>("A");
    Ports x = A->findMany<Port*>("./*[x]");
    QCOMPARE(x.size(), 2);
    QCOMPARE(x.at(0)->value<int>(), 20);
    QCOMPARE(x.at(1)->value<int>(), 30);
}

void TestPath::testFindBoxVsPort() {
    bool excepted(false);
    using Boxes = QVector<Box*>;
    using Ports = QVector<Port*>;
    Computation::changeStep(Computation::Step::Construct);

    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("S").
                box().name("A").endbox().
                box("Stage").name("B").endbox().
            endbox();
    }
    UNEXPECTED_EXCEPTION;
}
