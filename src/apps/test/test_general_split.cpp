#include <base/general.h>
#include "test_general_split.h"

using namespace base;

void TestGeneral::testEmpty() {
    QStringList list;
    list = split("");
    QCOMPARE(list, QStringList());
    list = split(" ");
    QCOMPARE(list, QStringList());
    list = split("  ");
    QCOMPARE(list, QStringList());
}

void TestGeneral::testOne() {
    QStringList list;
    list = split("A");
    QCOMPARE(list, QStringList() << "A");
    list = split("ABC");
    QCOMPARE(list, QStringList() << "ABC");
    list = split(" A");
    QCOMPARE(list, QStringList() << "A");
    list = split("A ");
    QCOMPARE(list, QStringList() << "A");
    list = split(" A ");
    QCOMPARE(list, QStringList() << "A");
    list = split(" ABC");
    QCOMPARE(list, QStringList() << "ABC");
    list = split("ABC ");
    QCOMPARE(list, QStringList() << "ABC");
    list = split(" ABC ");
    QCOMPARE(list, QStringList() << "ABC");
}

void TestGeneral::testTwo() {
    QStringList list;
    list = split("A B");
    QCOMPARE(list, QStringList() << "A" << "B");
    list = split(" A B");
    QCOMPARE(list, QStringList() << "A" << "B");
    list = split("A B ");
    QCOMPARE(list, QStringList() << "A" << "B");
    list = split(" A B ");
    QCOMPARE(list, QStringList() << "A" << "B");
}


void TestGeneral::testOneApEmpty() {
    QStringList list;
    list = split("\"\"");
    QCOMPARE(list, QStringList() << "");
    list = split(" \"\"");
    QCOMPARE(list, QStringList() << "");
    list = split("\"\"  ");
    QCOMPARE(list, QStringList() << "");
    list = split(" \"\"  ");
    QCOMPARE(list, QStringList() << "");
}

void TestGeneral::testOneApBlank() {
    QStringList list;
    list = split("\" \"");
    QCOMPARE(list, QStringList() << " ");
    list = split(" \" \"");
    QCOMPARE(list, QStringList() << " ");
    list = split("\"  \"  ");
    QCOMPARE(list, QStringList() << "  ");
    list = split(" \"  \"  ");
    QCOMPARE(list, QStringList() << "  ");
}

void TestGeneral::testMixed() {
    QStringList list;
    list = split("A \" B C \"");
    QCOMPARE(list, QStringList() << "A" << " B C ");

    list = split("A \"B\" C D \"E\" \"F\"");
    QCOMPARE(list, QStringList() << "A" << "B" << "C" << "D" << "E" << "F");
}

void TestGeneral::testWithParentheses() {
    QCOMPARE(split("A B C"), split("(A B C)"));
    QCOMPARE(split("A B C"), split("( A B C )"));
}
