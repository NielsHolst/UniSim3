/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/track.h>
#include "test_output_buffer.h"

using namespace base;

void TestOutputBuffer::testOk() {
    BoxBuilder builder;
    builder.
        box("Simulation").name("sim").
            port("steps").equals(5).
            box("Fibonacci").name("fibo").
            endbox().
            box("OutputBuffer").name("outputBuffer").
                port("ports").equals("(sim[step] fibo[value])").
            endbox().
        endbox();
    Box *sim;
    try {
        sim = builder.root();
        sim->run();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }

//    QFAIL("No track->buffer() method");
//    Port *value = sim->findOne<Port>("fibo[value]");
//    Track *track = Track::find(Track::Order{value->id(), PortFilter::None});
//    const Vector* vector = track->buffer();
//    const QVector<int> *values = reinterpret_cast<const QVector<int> *>(vector->ptr());

//    QCOMPARE(vector->size(), 6);
//    QCOMPARE(values->size(), 6);
//    QVector<int> expected;
//    expected << 1 << 1 << 2 << 3 << 5 << 8;
//    QCOMPARE(*values, expected);

    delete sim;
}

