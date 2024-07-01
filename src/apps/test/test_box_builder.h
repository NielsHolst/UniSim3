/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_BOX_BUILDER_H
#define TEST_BOX_BUILDER_H

#include "autotest.h"

namespace base {
    class  Box;
}

class TestBoxBuilder : public QObject
{
    Q_OBJECT
private slots:
    void testBuild1();
    void testBuild2();
};

DECLARE_TEST(TestBoxBuilder)

#endif
