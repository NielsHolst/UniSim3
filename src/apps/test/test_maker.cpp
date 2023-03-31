#include <iostream>
#include <base/box.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "output_file.h"
#include "test_maker.h"

using namespace base;

inline auto str(QString s) { return qPrintable(s + "\n"); }

void TestMaker::testReplicates() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/maker_replicates.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

//    std::cout << qPrintable(root->toText("ioa")) << std::endl;

    QVector<Box*> islands = root->findMany<Box*>("islands/*");
    QCOMPARE(islands.size(), 3);

    const Box *island0 = islands.at(0);
    const Box *island1 = islands.at(1);
    const Box *island2 = islands.at(2);
    QCOMPARE(island0->objectName(), QString("island0"));
    QCOMPARE(island1->objectName(), QString("island1"));
    QCOMPARE(island2->objectName(), QString("island2"));

    for (auto island : islands) {
        QVector<Box*> populations = island->findMany<Box*>("./*");
        QCOMPARE(populations.size(), 2);
        QCOMPARE(populations.at(0)->objectName(), QString("predator"));
        QCOMPARE(populations.at(1)->objectName(), QString("prey"));
    }

    // Must run OK
    try {
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));
}

void TestMaker::testNames() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/maker_names.box"));
        root = Box::root(builder.root());
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    QCOMPARE(Node::names(root->findMany<Box*>("system/*")), QStringList() << "X" << "Y" << "Z");
}
