#include <QFile>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/box_output.h>
#include <base/box_reader_boxes.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/port.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "test_ast_boxes.h"

using namespace base;

void TestAstBoxes::init() {
    builder = new BoxBuilder ;
    reader = new BoxReaderBoxes(builder);
}

void TestAstBoxes::cleanup(){
    delete builder;
    delete reader;
}

void TestAstBoxes::testSimple() {
    bool excepted(false);
    Box *root;
    try {
        reader->parse(inputFilePath("ast_simple.box"));
        root = builder->root();
    }
    UNEXPECTED
    BoxOutput output(root, BoxOutput::Indented);
    dialog().information(output.asText());
}

void TestAstBoxes::testComments() {
    bool excepted(false);
    Box *root;
    try {
        reader->parse(inputFilePath("ast_comments.box"));
        root = builder->root();
    }
    UNEXPECTED
    BoxOutput output(root, BoxOutput::Indented);
    dialog().information(output.asText());
}

void TestAstBoxes::testWhitespaceInVector() {
    bool excepted(false);
    Box *root;
    try {
        reader->parse(inputFilePath("ast_whitespace_in_vector.box"));
        root = builder->root();
    }
    UNEXPECTED

    typedef QVector<int> VecInt;
    QCOMPARE(root->findOne<Port*>("a[numbers1]")->value<VecInt>(),
             VecInt() << 7 << 9 << 13);
    QCOMPARE(root->findOne<Port*>("a[numbers1]")->value<VecInt>(),
             VecInt() << 7 << 9 << 13);

    typedef QVector<QString> VecString;
    QCOMPARE(root->findOne<Port*>("b[s1]")->value<VecString>(),
             VecString() << "abc" << "def" << "ghi");
    QCOMPARE(root->findOne<Port*>("b[s2]")->value<VecString>(),
             VecString() << "abc" << "def" << "ghi");
}

void TestAstBoxes::testWhitespaceInPath() {
    bool excepted(false);
    Box *root;
    Port *x, *y, *z;
    try {
        reader->parse(inputFilePath("ast_whitespace_in_path.box"));
        root = builder->root();
        root->run();
        x = root->findOne<Port*>("a[input2]");
        y = root->findOne<Port*>("a[input1]");
        z = root->findOne<Port*>("a [ input2 ]");
    }
    UNEXPECTED

    QCOMPARE(x->value<int>(), 7);
    QCOMPARE(y->value<int>(), 3);
    QCOMPARE(z->value<int>(), 7);
}

void TestAstBoxes::testTransform() {
    bool excepted(false);
    Box *root;
    Port *input1, *input2,
         *total, *mean, *min, *max, *any, *all, *all2;
    try {
        reader->parse(inputFilePath("ast_transform.box"));
        root = builder->root();
        root->run();
        input1 = root->findOne<Port>("a[input1]");
        input2 = root->findOne<Port>("a[input2]");
        total = root->findOne<Port>("a[total]");
        mean = root->findOne<Port>("a[mean]");
        min = root->findOne<Port>("a[min]");
        max = root->findOne<Port>("a[max]");
        any = root->findOne<Port>("a[any]");
        all = root->findOne<Port>("a[all]");
        all2 = root->findOne<Port>("a[all2]");
    }
    UNEXPECTED
    QCOMPARE(input1->value<int>(), 14);
    QCOMPARE(input2->value<int>(), 20);
    QCOMPARE(total->value<int>(), 31);
    QCOMPARE(mean->value<int>(), 7);
    QCOMPARE(min->value<int>(), 3);
    QCOMPARE(max->value<int>(), 17);
    QCOMPARE(any->value<bool>(), true);
    QCOMPARE(all->value<bool>(), false);
    QCOMPARE(all2->value<bool>(), true);
}



