#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/dialog.h>
#include <base/port_buffer.h>
#include "test_port_buffer.h"

using namespace std;
using namespace base;

void TestPortBuffer::init() {
    sim = nullptr;
}

void TestPortBuffer::cleanup() {
    delete sim;
}

void TestPortBuffer::testCreateBool() {
    QObject *parent = new QObject;

    bool *p = reinterpret_cast<bool*>( portBuffer(parent).createBuffer(Bool) ),
         *q = reinterpret_cast<bool*>( portBuffer(parent).createBuffer(Bool) );
    *p = true;
    *q = false;
    QVector<bool> *u = reinterpret_cast<QVector<bool>*>( portBuffer(parent).createBuffer(BoolVector) ),
                  *v = reinterpret_cast<QVector<bool>*>( portBuffer(parent).createBuffer(BoolVector) );
    *u << true << false;
    *v << false << true;

    delete parent;
}

//
// Scalar
//

void TestPortBuffer::testImportNoBuffer() {
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                box("ModelA").name("A").
                    port("input1").imports(".[input2]").
                    port("input2").equals(777).
                endbox().
            endbox();
        sim = builder.root();

        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QCOMPARE(sim->findOne<Port>("A[input1]")->value<int>(), 777);
}

void TestPortBuffer::testImportFromBuffer() {
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                box("ModelA").name("A").
                    port("input1").imports(".[input10]").
                    newPort("input10").equals(777).
                endbox().
            endbox();
        sim = builder.root();

        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QCOMPARE(sim->findOne<Port>("A[input1]")->value<int>(), 777);
}

void TestPortBuffer::testImportToBuffer() {
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                box("ModelA").name("A").
                    newPort("input10").imports(".[input2]").
                    port("input2").equals(777).
                endbox().
            endbox();
        sim = builder.root();

        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QCOMPARE(sim->findOne<Port>("A[input10]")->value<int>(), 777);
}

void TestPortBuffer::testImportBufferToBuffer() {
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                box("ModelA").name("A").
                    newPort("input10").imports(".[input20]").
                    newPort("input20").equals(777).
                endbox().
            endbox();
        sim = builder.root();

        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QCOMPARE(sim->findOne<Port>("A[input10]")->value<int>(), 777);
}

//
// Vector
//

void TestPortBuffer::testImportVectorNoBuffer() {
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                box("VectorInput").name("A").
                    port("numbers1").imports(".[numbers2]").
                    port("numbers2").equals("(7 13 9)").
                endbox().
            endbox();
        sim = builder.root();

        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QVector<int> expected;
    expected << 7 << 13 << 9;
    QCOMPARE(sim->findOne<Port>("A[numbers1]")->value<QVector<int>>(), expected);
}

void TestPortBuffer::testImportVectorFromBuffer() {
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                box("VectorInput").name("A").
                    port("numbers1").imports(".[numbers20]").
                    newPort("numbers20").equals("(7 13 9)").
                endbox().
            endbox();
        sim = builder.root();

        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QVector<int> expected;
    expected << 7 << 13 << 9;
    QCOMPARE(sim->findOne<Port>("A[numbers1]")->value<QVector<int>>(), expected);
}

void TestPortBuffer::testImportVectorToBuffer() {
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                box("VectorInput").name("A").
                    newPort("numbers10").imports(".[numbers2]").
                    port("numbers2").equals("(7 13 9)").
                endbox().
            endbox();
        sim = builder.root();

        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    QVector<int> expected;
    expected << 7 << 13 << 9;
    QCOMPARE(sim->findOne<Port>("A[numbers10]")->value<QVector<int>>(), expected);
}

void TestPortBuffer::testImportVectorBufferToBuffer() {
    BoxBuilder builder;
    QVector<int> actual, expected;
    try {
        builder.
            box("Simulation").name("sim").
                box("VectorInput").name("A").
                    newPort("numbers10").imports(".[numbers20]").
                    newPort("numbers20").equals("(7 13 9)").
                endbox().
            endbox();
        sim = builder.root();

        sim->amendFamily();
        sim->initializeFamily();
        sim->resetFamily();
        actual = sim->findOne<Port>("A[numbers10]")->value<QVector<int>>();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    expected << 7 << 13 << 9;
    QCOMPARE(actual, expected);
}
