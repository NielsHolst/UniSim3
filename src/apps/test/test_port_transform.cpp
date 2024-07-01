/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/exception.h>
#include "input_file_path.h"
#include "output_file.h"
#include "test_port_transform.h"

using namespace base;


void TestPortTransform::testSum() {
    BoxBuilder builder;
    bool excepted{false};
    Box *sim(nullptr);
    try {
        builder.
        box("Simulation").
            box("SeriesByIteration").name("series").
                port("numbers").equals("(5.0 7.0. 11.0)").
            endbox().
            box().name("sum").
                newPort("value").imports("series[numbers]").transform("Sum").
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
    double sum = sim->findOne<Box>("sum")->port("sum")->value<double>();
    QCOMPARE(sum, 23.0);
}

void TestPortTransform::testSumEmptySet() {

}


