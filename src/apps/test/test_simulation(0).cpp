#include <base/box.h>
#include <base/exception.h>
#include "test_box_cases.h"
#include "test_simulation.h"

using namespace base;

void TestSimulation::testDataFrame() {
    Box *sim;
    try {
        sim = TestBoxCases::case4();
        sim->amendFamily();
        sim->initializeFamily();
        sim->run();
    }
    catch (Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    delete sim;
}
