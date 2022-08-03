#include <base/box.h>
#include <base/box_builder.h>
#include <base/test_num.h>
#include "test_vector_sum.h"

using namespace base;

void TestVectorSum::testOk() {
    BoxBuilder builder;
    Box *sim(0);
    try {
        builder.
            box("Simulation").name("sim").
                box("boxes::VectorSum").name("sum").
                    port("input").equals("./*[vector]").
                    box().name("a").
                        newPort("vector").equals("(10.0 30.0 20.0)").
                    endbox().
                    box().name("b").
                        newPort("vector").equals("(1.0 3.0 2.0)").
                    endbox().
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
    QVector<double> sum = sim->findOne<Port>("sum[value]")->value<QVector<double>>();
    QCOMPARE(sum.size(), 3);
    QVERIFY(TestNum::eq(sum[0], 11.0));
    QVERIFY(TestNum::eq(sum[1], 33.0));
    QVERIFY(TestNum::eq(sum[2], 22.0));
    delete sim;
}

