/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/port.h>
#include <base/unique_names.h>
#include "test_unique_names.h"

using namespace base;
using namespace std;

void TestUniqueNames::testCombinations() {
    BoxBuilder builder;
    builder.
    box().name("root").
        box().name("X").
            box().name("A").
                aux("a").
                aux("b").
                aux("c").
            endbox().
        endbox().
        box().name("Y").
            box().name("A").
                aux("b").
                aux("c").
                aux("d").
            endbox().
        endbox().
        box().name("C").
            aux("a").
            aux("b").
            aux("c").
            aux("x").
        endbox().
        box().name("C").
            aux("d").
            aux("c").
            aux("b").
            aux("y").
        endbox().
        box().name("D").
            aux("a").
            aux("e").
            aux("d").
        endbox().
    endbox();
    auto root = std::unique_ptr<Box>( builder.root() );
    root->initializeFamily();

    QVector<Port*> ports = root->findMany<Port*>("*[*]");
    UniqueNames names(ports);
    auto unique = names.resolved();

    QStringList expected = {
        "A.a",
        "X.A.b",
        "X.A.c",
        "Y.A.b",
        "Y.A.c",
        "A.d",
        "C.a",
        "root.C.b.0",
        "root.C.c.0",
        "x",
        "C.d",
        "root.C.c.1",
        "root.C.b.1",
        "y",
        "D.a",
        "e",
        "D.d"};
    QCOMPARE(unique, expected);
}

void TestUniqueNames::testValueName() {
    BoxBuilder builder;
    builder.
    box().name("root").
        box().name("A").
            aux("x").
            aux("value").
        endbox().
    endbox();
    auto root = std::unique_ptr<Box>( builder.root() );
    root->initializeFamily();

    QVector<Port*> ports = root->findMany<Port*>("*[*]");
    UniqueNames names(ports);
    auto unique = names.resolved();

    QStringList expected = {"x","A"};
    QCOMPARE(unique, expected);
}

void TestUniqueNames::testUnnamedBox() {
    BoxBuilder builder;
    builder.
    box().
        box().name("A").
            aux("x").
            aux("y").
            box().
                aux("x").
            endbox().
            box().
                box().
                    aux("x").
                endbox().
            endbox().
        endbox().
    endbox();
    auto root = std::unique_ptr<Box>( builder.root() );
    root->initializeFamily();

    QVector<Port*> ports = root->findMany<Port*>("*[*]");
    UniqueNames names(ports);
    auto unique = names.resolved();

    QStringList expected = {"A.Box.x","Box.Box.x","A.x","y"};
    QCOMPARE(unique, expected);
}
