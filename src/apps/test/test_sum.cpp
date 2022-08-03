#include <base/box.h>
#include <base/box_builder.h>
#include <base/dialog.h>
#include <base/exception.h>
#include "test_sum.h"

using namespace base;


void TestSum::testWithValues() {
    BoxBuilder builder;
    bool excepted{false};
    Box *sim(nullptr);
    try {
        builder.
        box("Simulation").
            box().name("input").
                newPort("value").equals(5.0).
            endbox().
            box().name("input").
                newPort("value").equals(7.0).
            endbox().
            box().name("input").
                newPort("value").equals(11.0).
            endbox().
            box("Sum").name("sum").
                port("values").imports("input[value]").
            endbox().
        endbox();
        sim = builder.root();
        sim->run();
    }
    catch (Exception &ex) {
        excepted = true;
        dialog().information(ex.what());
    }
    QVERIFY(!excepted);
    QVERIFY(sim);
    double sum = sim->findOne<Box>("sum")->port("value")->value<double>();
    QCOMPARE(sum, 23.0);
}

void TestSum::testWithoutValues() {
    BoxBuilder builder;
    bool excepted{false};
    Box *sim(nullptr);
    try {
        builder.
        box("Simulation").
            box().name("input").
                newPort("value").equals(5.0).
            endbox().
            box().name("input").
                newPort("value").equals(7.0).
            endbox().
            box().name("input").
                newPort("value").equals(11.0).
            endbox().
            box("Sum").name("sum").
                port("values").imports("input[novalue!]").
            endbox().
        endbox();
        sim = builder.root();
        sim->run();
    }
    catch (Exception &ex) {
        excepted = true;
        dialog().information(ex.what());
    }
    QVERIFY(!excepted);
    QVERIFY(sim);
    double sum = sim->findOne<Box>("sum")->port("value")->value<double>();
    QCOMPARE(sum, 0.0);
}


