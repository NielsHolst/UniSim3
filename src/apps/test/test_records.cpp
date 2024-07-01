/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QVector>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/convert.h>
#include <base/environment.h>
#include "input_file_path.h"
#include "output_file.h"
#include "test_records.h"

using namespace base;
using namespace std;

void TestRecords::testDate() {
    environment().latestLoadArg("TestRecords_testDate.box");
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                port("steps").equals(5).
                box("Calendar").name("calendar").
                    port("initialDateTime").equals("13/11/1961").
                endbox().
                box("Records").name("records").
                    port("fileName").equals(inputFilePath("records/test_date.txt")).
                endbox().
                box("OutputText").name("text").
                    port("ports").equals("(calendar[date] calendar[time] records[Value])").
                endbox().
            endbox();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    Box *sim;
    try {
        sim = builder.root();
        sim->run();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    OutputFile file;
    QCOMPARE(file.numRows(), 6); // reset (1) + steps (6)

    QStringList dates = file.column("date"),
                expectedDates;
    expectedDates << "1961/11/13" << "1961/11/14" << "1961/11/15" << "1961/11/16" << "1961/11/17" << "1961/11/18";
    QCOMPARE(dates, expectedDates);

    QStringList values = file.column("Value"),
                expectedValues;
    expectedValues << "10.0" << "11.0" << "12.0" << "13.0" << "14.0" << "15.0";
    QCOMPARE(values, expectedValues);
    delete sim;
}

void TestRecords::testHour() {
    environment().latestLoadArg("TestRecords_testHour.box");
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                port("steps").equals(5).
                box("Calendar").name("calendar").
                    port("initialDateTime").equals("1/1/2001 13:00:00").
                    port("timeUnit").equals("h").
                endbox().
                box("Records").name("records").
                    port("fileName").equals(inputFilePath("records/test_hour.txt")).
                endbox().
                box("OutputText").name("text").
                    port("ports").equals("(calendar[date] calendar[time] records[Value])").
                endbox().
            endbox();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    Box *sim;
    try {
        sim = builder.root();
        sim->run();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    OutputFile file;
    QCOMPARE(file.numRows(), 6); // reset (1) + steps (6)

    QStringList hours = file.column("time"),
                expectedHours;
    expectedHours << "13:00:00" << "14:00:00" << "15:00:00" << "16:00:00" << "17:00:00" << "18:00:00";
    QCOMPARE(hours, expectedHours);

    QStringList values = file.column("Value"),
                expectedValues;
    expectedValues << "10.0" << "11.0" << "12.0" << "13.0" << "14.0" << "15.0";
    QCOMPARE(values, expectedValues);
    delete sim;
}

