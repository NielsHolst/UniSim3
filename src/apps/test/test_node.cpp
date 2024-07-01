/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <base/box.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "output_file.h"
#include "test_node.h"

#include <base/factory_plug_in.h>
#include <base/mega_factory.h>

using namespace base;

inline auto str(QString s) { return qPrintable(s + "\n"); }

void TestNode::testClassName() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/class_name.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    QCOMPARE(root->findOne<Box*>("box")->pedigree().size(), 1);
    QCOMPARE(root->findOne<Box*>("box")->className(), "Box");
    QCOMPARE(root->findOne<Box*>("box")->className(Node::Namespace::Include), "base::Box");
    QVERIFY(root->findOne<Box*>("box")->isType("Box"));
    QVERIFY(root->findOne<Box*>("box")->isType("base::Box"));

    QCOMPARE(root->findOne<Box*>("stage")->pedigree().size(), 2);
    QCOMPARE(root->findOne<Box*>("stage")->className(), "Stage");
    QCOMPARE(root->findOne<Box*>("stage")->className(Node::Namespace::Include), "boxes::Stage");
    QVERIFY(root->findOne<Box*>("stage")->isType("Box"));
    QVERIFY(root->findOne<Box*>("stage")->isType("base::Box"));
    QVERIFY(root->findOne<Box*>("stage")->isType("Stage"));
    QVERIFY(root->findOne<Box*>("stage")->isType("boxes::Stage"));

    QCOMPARE(root->findOne<Box*>("randomiser")->pedigree().size(), 3);
    QCOMPARE(root->findOne<Box*>("randomiser")->className(), "RandomiserMonteCarlo");
    QVERIFY(root->findOne<Box*>("randomiser")->isType("Box"));
    QVERIFY(root->findOne<Box*>("randomiser")->isType("RandomiserBase"));
    QVERIFY(root->findOne<Box*>("randomiser")->isType("RandomiserMonteCarlo"));

    QCOMPARE(root->findOne<Box*>("rnd")->pedigree().size(), 3);
    QCOMPARE(root->findOne<Box*>("rnd")->className(), "RandomUniform");
    QVERIFY(root->findOne<Box*>("rnd")->isType("Box"));
    QVERIFY(root->findOne<Box*>("rnd")->isType("RandomBase"));
    QVERIFY(root->findOne<Box*>("rnd")->isType("RandomUniform"));
}
