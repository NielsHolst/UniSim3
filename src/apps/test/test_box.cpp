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
#include "test_box.h"

using namespace base;

inline auto str(QString s) { return qPrintable(s + "\n"); }

void TestBox::testClone() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/box_clone.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    Box *A = root->findOne<Box*>("A");
    try {
        A->cloneFamily("Aclone", root);
    }
    UNEXPECTED_EXCEPTION;
    // std::cout << qPrintable(root->toText("ioa")) << std::endl;

    QCOMPARE(Node::names(root->findMany<Box*>("./*")), QStringList() << "A" << "Aclone");

    QCOMPARE(root->findOne<Port*>(     "A/B/D[inflow]")->value<double>(), 1000.);
    QCOMPARE(root->findOne<Port*>("Aclone/B/D[inflow]")->value<double>(), 1000.);

    QCOMPARE(root->findOne<Port*>(     "A/B/D[k]")->expression().originalAsString(true), QString("..[b]{Path}"));
    QCOMPARE(root->findOne<Port*>("Aclone/B/D[k]")->expression().originalAsString(true), QString("..[b]{Path}"));

    QCOMPARE(root->findOne<Port*>(     "A[a]")->value<int>(), 13);
    QCOMPARE(root->findOne<Port*>("Aclone[a]")->value<int>(), 13);

    QCOMPARE(root->findOne<Port*>(     "A/B[b]")->value<int>(), 14);
    QCOMPARE(root->findOne<Port*>("Aclone/B[b]")->value<int>(), 14);

    QCOMPARE(root->findOne<Port*>(     "A/C[c]")->expression().originalAsString(true), QString("D[k]{Path}"));
    QCOMPARE(root->findOne<Port*>("Aclone/C[c]")->expression().originalAsString(true), QString("D[k]{Path}"));

}
