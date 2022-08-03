#include <iostream>
#include <base/history.h>
#include "test_history.h"

using namespace base;

void TestHistory::testAddOne() {
    History history(5);
    QCOMPARE(history.previous(), QString() );
    QCOMPARE(history.next(), QString() );
    history.add("moon stone");
    QCOMPARE(history.previous(), QString("moon stone"));
    QCOMPARE(history.next(), QString("moon stone"));
}

void TestHistory::testOverFullHistoryPrevious() {
    History history(3);
    QCOMPARE(history.previous(), QString());
    history.add("moon stone A");
    QCOMPARE(history.previous(), QString("moon stone A"));
    QCOMPARE(history.previous(), QString("moon stone A"));
    history.add("moon stone B");
    history.add("moon stone C");
    history.add("moon stone D");
    QCOMPARE(history.previous(), QString("moon stone D"));
    QCOMPARE(history.previous(), QString("moon stone C"));
    QCOMPARE(history.previous(), QString("moon stone B"));
    QCOMPARE(history.previous(), QString("moon stone D"));
    QCOMPARE(history.previous(), QString("moon stone C"));
    QCOMPARE(history.next(), QString("moon stone D"));
}

void TestHistory::testNotFullHistoryPrevious() {
    History history(10);
    QCOMPARE(history.previous(), QString());
    history.add("moon stone A");
    QCOMPARE(history.previous(), QString("moon stone A"));
    QCOMPARE(history.previous(), QString("moon stone A"));
    history.add("moon stone B");
    history.add("moon stone C");
    history.add("moon stone D");
    QCOMPARE(history.previous(), QString("moon stone D"));
    QCOMPARE(history.previous(), QString("moon stone C"));
    QCOMPARE(history.previous(), QString("moon stone B"));
    QCOMPARE(history.previous(), QString("moon stone A"));
    QCOMPARE(history.previous(), QString("moon stone D"));
    QCOMPARE(history.next(), QString("moon stone A"));
}

void TestHistory::testJustFullHistoryPrevious() {
    History history(4);
    QCOMPARE(history.previous(), QString());
    history.add("moon stone A");
    QCOMPARE(history.previous(), QString("moon stone A"));
    QCOMPARE(history.previous(), QString("moon stone A"));
    history.add("moon stone B");
    history.add("moon stone C");
    history.add("moon stone D");
    QCOMPARE(history.previous(), QString("moon stone D"));
    QCOMPARE(history.previous(), QString("moon stone C"));
    QCOMPARE(history.previous(), QString("moon stone B"));
    QCOMPARE(history.previous(), QString("moon stone A"));
    QCOMPARE(history.previous(), QString("moon stone D"));
    QCOMPARE(history.next(), QString("moon stone A"));
}

void TestHistory::testOverFullHistoryNext() {
    History history(3);
    QCOMPARE(history.next(), QString());
    history.add("moon stone A");
    QCOMPARE(history.next(), QString("moon stone A"));
    QCOMPARE(history.next(), QString("moon stone A"));
    history.add("moon stone B");
    history.add("moon stone C");
    history.add("moon stone D");
    QCOMPARE(history.next(), QString("moon stone B"));
    QCOMPARE(history.next(), QString("moon stone C"));
    QCOMPARE(history.next(), QString("moon stone D"));
    QCOMPARE(history.next(), QString("moon stone B"));
    QCOMPARE(history.next(), QString("moon stone C"));
    QCOMPARE(history.previous(), QString("moon stone B"));
}

void TestHistory::testNotFullHistoryNext() {
    History history(10);
    QCOMPARE(history.next(), QString());
    history.add("moon stone A");
    QCOMPARE(history.next(), QString("moon stone A"));
    QCOMPARE(history.next(), QString("moon stone A"));
    history.add("moon stone B");
    history.add("moon stone C");
    history.add("moon stone D");
    QCOMPARE(history.next(), QString("moon stone A"));
    QCOMPARE(history.next(), QString("moon stone B"));
    QCOMPARE(history.next(), QString("moon stone C"));
    QCOMPARE(history.next(), QString("moon stone D"));
    QCOMPARE(history.next(), QString("moon stone A"));
    QCOMPARE(history.previous(), QString("moon stone D"));
}

void TestHistory::testJustFullHistoryNext() {
    History history(4);
    QCOMPARE(history.next(), QString());
    history.add("moon stone A");
    QCOMPARE(history.next(), QString("moon stone A"));
    QCOMPARE(history.next(), QString("moon stone A"));
    history.add("moon stone B");
    history.add("moon stone C");
    history.add("moon stone D");
    QCOMPARE(history.next(), QString("moon stone A"));
    QCOMPARE(history.next(), QString("moon stone B"));
    QCOMPARE(history.next(), QString("moon stone C"));
    QCOMPARE(history.next(), QString("moon stone D"));
    QCOMPARE(history.next(), QString("moon stone A"));
    QCOMPARE(history.previous(), QString("moon stone D"));
}
