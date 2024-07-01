/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_PORT_VALUE_OP_H
#define TEST_PORT_VALUE_OP_H

#include "autotest.h"

class TestPortValueOp : public QObject
{
    Q_OBJECT
private slots:
    void testAllocate();
    void testAccumulate();
    void testDivide();
    void testAdd();
    void testMin();
    void testMax();
    void testLt();
    void testGt();
    void testEq();
    void testLe();
    void testGe();
};

DECLARE_TEST(TestPortValueOp)

#endif
