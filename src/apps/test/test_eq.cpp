#include <base/box.h>
#include <base/box_builder.h>
#include <base/port.h>
#include <base/test_num.h>
#include "test_eq.h"

using namespace base;

void TestEq::testGompertz() {
    BoxBuilder builder;
    Box *sim(0);
    QVector<double> x, result, expected;
    x << 30 << 60 << 90;
    expected << 0.2762402 << 0.3705556 << 0.3924199;
    try {
        builder.
            box("Simulation").name("sim").
                port("steps").equals(2).
                box("eq::Gompertz").name("gompertz").
                    port("x").equals(x).
                    port("dx").equals(10).
                endbox().
            endbox();

        sim = builder.root();
        sim->run();
    }
    catch(Exception &ex) {
        ThrowException(QString("Unexpected exception:\n") + ex.what());
    }

    result = sim->findOne<Port>("gompertz[dy]")->value<QVector<double>>();
    QVERIFY(TestNum::eq(result.at(0), expected.at(0)));
    QVERIFY(TestNum::eq(result.at(1), expected.at(1)));
    QVERIFY(TestNum::eq(result.at(2), expected.at(2)));

    delete sim;
}

