#include <base/data_frame.h>
#include "input_file_path.h"
#include "test_data_frame.h"

using namespace base;

void TestDataFrame::testDimensions() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df1.txt"), Table::BothLabelled);
        QCOMPARE(df.numCol(), 2);
        QCOMPARE(df.numRow(), 2);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestDataFrame::testContentBothLabelled() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df2.txt"), Table::BothLabelled);
        QCOMPARE(df.numCol(), 2);
        QCOMPARE(df.numRow(), 2);

        QCOMPARE(df.colNames(), QStringList() << "ColB" << "ColC");
        QCOMPARE(df.ixCol("ColB"), 0);
        QCOMPARE(df.ixCol("ColC"), 1);

        QCOMPARE(df.rowNames(), QStringList() << "Row1" << "Row2");
        QCOMPARE(df.ixRow("Row1"), 0);
        QCOMPARE(df.ixRow("Row2"), 1);

        QCOMPARE(df.row(0), QVector<QString>() << "7" << "9");
        QCOMPARE(df.row(1), QVector<QString>() << "A B" << "111");

        QCOMPARE(df.col(0), QVector<QString>() << "7" << "A B");
        QCOMPARE(df.col(1), QVector<QString>() << "9" << "111");

        QCOMPARE(df(0,0), QString("7"));
        QCOMPARE(df(1,1), QString("111"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestDataFrame::testContentColumnLabelled() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df2.txt"), DataFrame::ColumnLabelled);
        QCOMPARE(df.numCol(), 3);
        QCOMPARE(df.numRow(), 2);

        QCOMPARE(df.colNames(), QStringList() << "ColA" << "ColB" << "ColC");
        QCOMPARE(df.ixCol("ColA"), 0);
        QCOMPARE(df.ixCol("ColB"), 1);
        QCOMPARE(df.ixCol("ColC"), 2);

        QVERIFY(df.rowNames().isEmpty());

        QCOMPARE(df.row(0), QVector<QString>() << "Row1" <<"7" << "9");
        QCOMPARE(df.row(1), QVector<QString>() << "Row2" << "A B" << "111");

        QCOMPARE(df.col(0), QVector<QString>() << "Row1" << "Row2");
        QCOMPARE(df.col(1), QVector<QString>() << "7" << "A B");
        QCOMPARE(df.col(2), QVector<QString>() << "9" << "111");

        QCOMPARE(df(0,0), QString("Row1"));
        QCOMPARE(df(1,1), QString("A B"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestDataFrame::testContentRowLabelled() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df2.txt"), Table::RowLabelled);
        QCOMPARE(df.numCol(), 2);
        QCOMPARE(df.numRow(), 3);

        QVERIFY(df.colNames().isEmpty());

        QCOMPARE(df.rowNames(), QStringList() << "ColA" << "Row1" << "Row2");
        QCOMPARE(df.ixRow("ColA"), 0);
        QCOMPARE(df.ixRow("Row1"), 1);
        QCOMPARE(df.ixRow("Row2"), 2);

        QCOMPARE(df.row(0), QVector<QString>() << "ColB" << "ColC");
        QCOMPARE(df.row(1), QVector<QString>() << "7" << "9");
        QCOMPARE(df.row(2), QVector<QString>() << "A B" << "111");

        QCOMPARE(df.col(0), QVector<QString>() << "ColB" << "7" << "A B");
        QCOMPARE(df.col(1), QVector<QString>() << "ColC" << "9" << "111");

        QCOMPARE(df(0,0), QString("ColB"));
        QCOMPARE(df(1,1), QString("9"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestDataFrame::testContentNoLabelling() {
    DataFrame df;
    try {
        df.read( inputFilePath("data_frame/df2.txt"), DataFrame::NoLabelling);
        QCOMPARE(df.numCol(), 3);
        QCOMPARE(df.numRow(), 3);

        QVERIFY(df.colNames().isEmpty());
        QVERIFY(df.rowNames().isEmpty());
        QCOMPARE(df.row(0), QVector<QString>() << "ColA" << "ColB" << "ColC");
        QCOMPARE(df.row(1), QVector<QString>() << "Row1" << "7" << "9");
        QCOMPARE(df.row(2), QVector<QString>() << "Row2" << "A B" << "111");

        QCOMPARE(df.col(0), QVector<QString>() << "ColA" << "Row1" << "Row2");
        QCOMPARE(df.col(1), QVector<QString>() << "ColB" << "7" << "A B");
        QCOMPARE(df.col(2), QVector<QString>() << "ColC" << "9" << "111");

        QCOMPARE(df(0,0), QString("ColA"));
        QCOMPARE(df(1,1), QString("7"));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestDataFrame::testAsInt() {
    DataFrame df;
    QVector<int> i;
    try {
        df.read( inputFilePath("data_frame/df1.txt"), Table::BothLabelled);
        i = df.row<int>(0);
        QCOMPARE(i, QVector<int>() << 7 << 9);
        i = df.row<int>(1);
        QCOMPARE(i, QVector<int>() << 88 << 111);
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}
