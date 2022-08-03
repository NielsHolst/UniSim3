#include <iostream>
#include <boost/circular_buffer.hpp>
#include <base/circular_buffer.h>
#include "test_circle_buffer.h"

void TestCircleBuffer::testStdVersion() {
    // Create a circular buffer with a capacity for 3 integers.
    boost::circular_buffer<int> cb(3);

    // Insert threee elements into the buffer.
    cb.push_back(1);
    cb.push_back(2);
    std::cout << "\nA\n";
    for (auto it : cb) std::cout << it << "\n";
    cb.push_back(3);

    int a = cb[0];  // a == 1
    int b = cb[1];  // b == 2
    int c = cb[2];  // c == 3
    QCOMPARE(a, 1);
    QCOMPARE(b, 2);
    QCOMPARE(c, 3);

    // The buffer is full now, so pushing subsequent
    // elements will overwrite the front-most elements.

    cb.push_back(4);  // Overwrite 1 with 4.
    cb.push_back(5);  // Overwrite 2 with 5.

    // The buffer now contains 3, 4 and 5.
    a = cb[0];  // a == 3
    b = cb[1];  // b == 4
    c = cb[2];  // c == 5
    QCOMPARE(a, 3);
    QCOMPARE(b, 4);
    QCOMPARE(c, 5);

    std::cout << "\nB\n";
    for (auto it : cb) std::cout << it << "\n";
}

void TestCircleBuffer::testUniSimVersion() {
    QVector<int> buffer;
    base::CircularBuffer<int> cb(&buffer);

    cb.resize(3);
    QVERIFY(!cb.isFull());
    QCOMPARE(cb.sum(), 0);
    QCOMPARE(cb.average(), 0);
    QVERIFY(cb.min() > 10000);
    QVERIFY(cb.max() < -10000);

    cb.push(1);
    QVERIFY(cb.head() == 1);
    QVERIFY(cb.tail() == 1);
    QVERIFY(!cb.isFull());
    QCOMPARE(cb.sum(), 1);
    QCOMPARE(cb.average(), 1);
    QCOMPARE(cb.min(), 1);
    QCOMPARE(cb.max(), 1);

    cb.push(2);
    QVERIFY(cb.head() == 2);
    QVERIFY(cb.tail() == 1);
    QVERIFY(!cb.isFull());
    QCOMPARE(cb.sum(), 3);
    QCOMPARE(cb.average(), 1);
    QCOMPARE(cb.min(), 1);
    QCOMPARE(cb.max(), 2);

    cb.push(3);
    QVERIFY(cb.head() == 3);
    QVERIFY(cb.tail() == 1);
    QVERIFY(cb.isFull());
    QCOMPARE(cb.sum(), 6);
    QCOMPARE(cb.average(), 2);
    QCOMPARE(cb.min(), 1);
    QCOMPARE(cb.max(), 3);
    QCOMPARE(buffer, QVector<int>() << 1 << 2 << 3);
    QCOMPARE(cb.at(0), 3);
    QCOMPARE(cb.at(2), 2);

    cb.push(4);
    QVERIFY(cb.head() == 4);
    QVERIFY(cb.tail() == 2);
    QVERIFY(cb.isFull());
    QCOMPARE(cb.sum(), 9);
    QCOMPARE(cb.average(), 3);
    QCOMPARE(cb.min(), 1);
    QCOMPARE(cb.max(), 4);
    QCOMPARE(buffer, QVector<int>() << 4 << 2 << 3);
    QCOMPARE(cb.at(0), 4);
    QCOMPARE(cb.at(1), 2);
    QCOMPARE(cb.at(2), 3);

    cb.push(5);
    QCOMPARE(buffer, QVector<int>() << 4 << 5 << 3);
    cb.set(0,7);
    QCOMPARE(buffer, QVector<int>() << 4 << 7 << 3);
    QCOMPARE(cb.sum(), 14);
    QCOMPARE(cb.average(), 4);
    QCOMPARE(cb.min(), 1);
    QCOMPARE(cb.max(), 7);
}
