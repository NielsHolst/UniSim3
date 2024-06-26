/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/flags.h>
#include "test_flags.h"

//
// Simple implementation
//

enum class Colours {Red=1, Blue=2, Green=4};

Colours operator |(Colours lhs, Colours rhs)
{
    return static_cast<Colours> (
        static_cast<unsigned>(lhs) |
        static_cast<unsigned>(rhs)
    );
}

bool operator &(Colours lhs, Colours rhs)
{
    return static_cast<bool> (
        static_cast<unsigned>(lhs) &
        static_cast<unsigned>(rhs)
    );
}

void TestFlags::testColours() {
    Colours colour = Colours::Red | Colours::Blue;
    QVERIFY(colour & Colours::Red);
    QVERIFY(!(colour & Colours::Green));
}

enum class Tastes{Sour=1, Sweet=2, Bitter=4};
ENABLE_FLAGS(Tastes);

void TestFlags::testTastes() {
    Tastes taste = Tastes::Sweet | Tastes::Bitter;
    QVERIFY(taste & Tastes::Bitter);
    QVERIFY(!(taste & Tastes::Sour));
}
