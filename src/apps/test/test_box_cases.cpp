/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <base/bare_date.h>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/computation.h>
#include <base/exception.h>
#include "test_box_cases.h"

using namespace base;
using namespace std;

namespace TestBoxCases {

Box* case1() {
    Computation::changeStep(Computation::Step::Construct);
    BoxBuilder builder;
    builder.
            box().name("A").
            endbox();
    return builder.root();
}

Box* case2() {
    Computation::changeStep(Computation::Step::Construct);
    BoxBuilder builder;
    builder.
        box().name("A").
            aux("v1").equals(QString("ape")).
            aux("v3").equals("monkey").
            box().name("A1").
                aux("v1").equals(100).
                box().name("a").
                    aux("v1").equals(true).
                endbox().
                box().name("a").
                    aux("v1").equals(2.13).
                endbox().
                box().name("c").
                    aux("v2").equals(QDate(2021, 12, 24)).
                endbox().
            endbox().
            box().name("A2").
                aux("v2").equals(0).
                box().name("a").endbox().
                box().name("b").
                    aux("v1").equals(QVector<int>() << 4 << 2 << 7).
                endbox().
                box().name("c").
                    aux("v2").equals(QVector<double>() << 4.3 << 2.1 << -7.7).
                endbox().
            endbox().
            box().name("A3").
                aux("v3").equals(QTime(13,14,15)).
                box().name("a").
                    aux("v2").equals(QVector<QTime>() << QTime(13,14,15) << QTime(16,17,18)).
                endbox().
                box().name("b").
                    aux("v3").equals(BareDate(7,13)).
                endbox().
            endbox().
        endbox();
    return builder.root();
}


} // namespace

