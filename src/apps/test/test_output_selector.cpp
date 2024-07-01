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
#include "test_output_selector.h"

using namespace base;

inline auto str(QString s) { return qPrintable(s); }

void TestOutputSelector::testPeriod1Iter() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/output_selector_period_1_iter.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    OutputFile f(OutputFile::Format::WithoutFormatLine);
    QCOMPARE(f.numRows(), 5);

    QVector<int>
        iteration  = f.column<int>("iteration"),
        iterationE = QVector<int>(5,1),
        step       = f.column<int>("step"),
        stepE      = {0,6,12,18,24};
    QCOMPARE(iteration, iterationE);
    QCOMPARE(step, stepE);
}

void TestOutputSelector::testPeriodNIter() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/output_selector_period_n_iter.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    OutputFile f(OutputFile::Format::WithoutFormatLine);
    QCOMPARE(f.numRows(), 15);

    QVector<int>
        iteration  = f.column<int>("iteration"),
        iterationE = {1,1,1,1,1,2,2,2,2,2,3,3,3,3,3},
        step       = f.column<int>("step"),
        stepE      = {0,6,12,18,24,0,6,12,18,24,0,6,12,18,24};
    QCOMPARE(iteration, iterationE);
    QCOMPARE(step, stepE);
}

void TestOutputSelector::testFinal1Iter() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/output_selector_final_1_iter.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    OutputFile f(OutputFile::Format::WithoutFormatLine);
    QCOMPARE(f.numRows(), 1);

    QVector<int>
        iteration  = f.column<int>("iteration"),
        iterationE = {1},
        step       = f.column<int>("step"),
        stepE      = {24};
    QCOMPARE(iteration, iterationE);
    QCOMPARE(step, stepE);
}

void TestOutputSelector::testFinalNIter() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/output_selector_final_n_iter.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    OutputFile f(OutputFile::Format::WithoutFormatLine);
    QCOMPARE(f.numRows(), 3);

    QVector<int>
        iteration  = f.column<int>("iteration"),
        iterationE = {1,2,3},
        step       = f.column<int>("step"),
        stepE      = {24,24,24};
    QCOMPARE(iteration, iterationE);
    QCOMPARE(step, stepE);
}


void TestOutputSelector::testPeriod1IterSkip() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/output_selector_period_1_iter_skip.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    OutputFile f(OutputFile::Format::WithoutFormatLine);
    QCOMPARE(f.numRows(), 4);

    QVector<int>
        iteration  = f.column<int>("iteration"),
        iterationE = QVector<int>(4,1),
        step       = f.column<int>("step"),
        stepE      = {3,9,15,21};
    QCOMPARE(iteration, iterationE);
    QCOMPARE(step, stepE);
}

void TestOutputSelector::testPeriodNIterSkip() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/output_selector_period_n_iter_skip.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    OutputFile f(OutputFile::Format::WithoutFormatLine);
    QCOMPARE(f.numRows(), 12);

    QVector<int>
        iteration  = f.column<int>("iteration"),
        iterationE = {1,1,1,1,2,2,2,2,3,3,3,3},
        step       = f.column<int>("step"),
        stepE      = {3,9,15,21,3,9,15,21,3,9,15,21};
    QCOMPARE(iteration, iterationE);
    QCOMPARE(step, stepE);
}
