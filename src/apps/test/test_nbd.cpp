/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <base/nbd.h>
#include <base/test_num.h>
#include "test_nbd.h"

using namespace base;
using TestNum::eq;

void TestNbd::testProbability() {
    Nbd nbd(1.5, 10.);
    double
        expected[8] = {
           0.2471847100,
           0.3224148300,
           0.2312976000,
           0.1206770100,
           0.0511565580,
           0.0186832650,
           0.0060923689,
           0.0018163584
        };

    for (int i=0; i<8; ++i) {
        const double prob = nbd.nextProbability();
        // std::cout << i << " " << prob << " " << expected[i] << "\n";
        QVERIFY(eq(prob, expected[i]));
    }
}

