#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/computation.h>
#include <base/port.h>
#include "exception_expectation.h"
#include "test_date_time_signal.h"

using namespace base;
using namespace std;

Box* buildDates(QString currentDateTime) {
    BoxBuilder builder;
    Computation::changeStep(Computation::Step::Construct);
    builder.
    box("Simulation").name("sim").
        box("DateTimeSignal").name("signal").
            port("beginDate").equals("01/10").
            port("endDate").  equals("31/10").
            port("currentDateTime").equals(currentDateTime).
        endbox().
    endbox();
    return Box::root(builder.root());
}

Box* buildDatesWinter(QString currentDateTime) {
    BoxBuilder builder;
    Computation::changeStep(Computation::Step::Construct);
    builder.
    box("Simulation").name("sim").
        box("DateTimeSignal").name("signal").
            port("beginDate").equals("01/12").
            port("endDate").  equals("28/02").
            port("currentDateTime").equals(currentDateTime).
        endbox().
    endbox();
    return Box::root(builder.root());
}

Box* buildTimes(QString currentDateTime) {
    BoxBuilder builder;
    Computation::changeStep(Computation::Step::Construct);
    builder.
    box("Simulation").name("sim").
        box("DateTimeSignal").name("signal").
            port("beginTime").equals("08:00").
            port("endTime").  equals("16:00").
            port("currentDateTime").equals(currentDateTime).
        endbox().
    endbox();
    return Box::root(builder.root());
}

Box* buildDateTimes(QString currentDateTime) {
    BoxBuilder builder;
    Computation::changeStep(Computation::Step::Construct);
    builder.
    box("Simulation").name("sim").
        box("DateTimeSignal").name("signal").
            port("circadian").equals(false).
            port("beginDate").equals("01/10").
            port("beginTime").equals("08:00").
            port("endDate").  equals("31/10").
            port("endTime").  equals("16:00").
            port("currentDateTime").equals(currentDateTime).
        endbox().
    endbox();
    return Box::root(builder.root());
}

void TestDateTimeSignal::testBeforeDates() {
    Box *root = buildDates("15/09/2017");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(!inside);
}

void TestDateTimeSignal::testInsideDates() {
    Box *root = buildDates("15/10/2017");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(inside);
}


void TestDateTimeSignal::testAfterDates() {
    Box *root = buildDates("15/12/2017");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(!inside);
}

void TestDateTimeSignal::testBeforeDatesWinter() {
    Box *root = buildDatesWinter("15/09/2017");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(!inside);
}

void TestDateTimeSignal::testInsideDatesWinter() {
    Box *root = buildDatesWinter("15/12/2017");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(inside);
}


void TestDateTimeSignal::testAfterDatesWinter() {
    Box *root = buildDatesWinter("15/05/2017");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(!inside);
}

void TestDateTimeSignal::testBeforeTimes() {
    Box *root = buildTimes("15/09/2017T07:15");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(!inside);
}

void TestDateTimeSignal::testInsideTimes() {
    Box *root = buildTimes("15/09/2017T12:15");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(inside);
}

void TestDateTimeSignal::testAfterTimes() {
    Box *root = buildTimes("15/09/2017T021:45");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(!inside);
}

void TestDateTimeSignal::testBeforeFirstDateTime() {
    Box *root = buildDateTimes("01/10/2017T07:00");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(!inside);
}

void TestDateTimeSignal::testInsideFirstDateTime() {
    Box *root = buildDateTimes("01/10/2017T09:00");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(inside);
}

void TestDateTimeSignal::testInsideLastDateTime() {
    Box *root = buildDateTimes("31/10/2017T15:00");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(inside);
}

void TestDateTimeSignal::testAfterLastDateTime() {
    Box *root = buildDateTimes("31/10/2017T17:00");
    root->amendFamily();
    root->run();
    QVERIFY(noRuntimeError(root));
    bool inside = root->findOne<Box*>("signal")->port("value")->value<bool>();
    QVERIFY(!inside);
}
