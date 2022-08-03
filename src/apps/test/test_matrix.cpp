#include <base/data_frame.h>
#include <base/matrix.h>
#include "input_file_path.h"
#include "test_matrix.h"

using namespace base;

void TestMatrix::testInt() {
    DataFrame df;
    QVector<int> i;
    try {
        df.read( inputFilePath("matrix/matrix-int.txt"), Table::BothLabelled);
        i = df.row<int>(0);
        QCOMPARE(i, QVector<int>() << 7 << 9);
        i = df.row<int>(1);
        QCOMPARE(i, QVector<int>() << 88 << 111);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestMatrix::testString() {
    DataFrame df;
    QVector<QString> s;
    try {
        df.read( inputFilePath("matrix/matrix-string.txt"), Table::BothLabelled);
        s = df.row<QString>(0);
        QCOMPARE(s, QVector<QString>() << "7" << "9");
        s = df.row<QString>(1);
        QCOMPARE(s, QVector<QString>() << "A B"  << "111");
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestMatrix::testResize() {
    Matrix<int> M;
    M.resize(2,3);
    QCOMPARE(M.numRow(), 2);
    QCOMPARE(M.numCol(), 3);
    QCOMPARE(M.rowNames(), QStringList());
    QCOMPARE(M.colNames(), QStringList());
    QCOMPARE(M.row(0), QVector<int>() << 0 << 0 << 0);
    QCOMPARE(M.row(1), QVector<int>() << 0 << 0 << 0);

    M(0,0) = 2;
    M(0,1) = 4;
    M(0,2) = 6;
    M(1,0) = 8;
    M(1,1) = 10;
    M(1,2) = 12;

    QCOMPARE(M.at(0,0), 2);
    QCOMPARE(M.at(0,1), 4);
    QCOMPARE(M.at(0,2), 6);
    QCOMPARE(M.at(1,0), 8);
    QCOMPARE(M.at(1,1), 10);
    QCOMPARE(M.at(1,2), 12);
}
