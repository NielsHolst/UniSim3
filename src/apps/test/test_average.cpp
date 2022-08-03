#include <iostream>
#include <base/box.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "output_file.h"
#include "test_average.h"

using namespace base;

inline auto str(QString s) { return qPrintable(s); }



template <class T>
inline void dump(std::string s, QVector<T> v) {
    std::cout << s << std::endl;
    for (auto x : v)
        std::cout << x << std::endl;
}

inline bool compare(QVector<double> a, QVector<double> b) {
    if (a.size() != b.size())
        return false;
    for (int i=0; i<a.size(); ++i) {
        if (fabs(a.at(i) - b.at(i)) > 1e-3) {
            std::cout << "Unequal values in vectors: " << a.at(i) << "!=" << b.at(i)
                      << " diff = " << a.at(i)-b.at(i) << std::endl;
            return false;
        }
    }
    return true;
}


void TestAverage::testPeriod1() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/average_period_1.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    OutputFile f(OutputFile::Format::WithoutFormatLine);
//    std::cout << "*testPeriod1 file: " << qPrintable(f.filePath()) << "\n";
    QCOMPARE(f.numRows(), 42);

    QVector<double>
        step      = f.column<double>("sim.step"),
        square    = f.column<double>("sim.square"),
        avgStep   = f.column<double>("average.step"),
        avgSquare = f.column<double>("average.square");
    QCOMPARE(step,   avgStep);
    QCOMPARE(square, avgSquare);
}

void TestAverage::testPeriodN() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/average_period_n.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    OutputFile f(OutputFile::Format::WithoutFormatLine);

    QCOMPARE(f.numRows(), 8);
    QVector<double>
        avgStep    = f.column<double>("average.step"),
        avgSquare  = f.column<double>("average.square"),
        avgStepE   = {
                      0.,
                      (1+2+3+4+5+6)/6.,
                      (7+8+9+10+11+12)/6.,
                      (13+14+15+16+17+18)/6.,
                      0.,
                      (1+2+3+4+5+6)/6.,
                      (7+8+9+10+11+12)/6.,
                      (13+14+15+16+17+18)/6.
                       },
        avgSquareE = {
                      0.,
                      (1+4+9+16+25+36)/6.,
                      (49+64+81+100+121+144)/6.,
                      (169+196+225+256+289+324)/6.,
                      0.,
                      (1+4+9+16+25+36)/6.,
                      (49+64+81+100+121+144)/6.,
                      (169+196+225+256+289+324)/6.
    };
    QVERIFY(compare(avgStep, avgStepE));
    QVERIFY(compare(avgSquare, avgSquareE));
}
