/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_PHYS_MATH_H
#define TEST_PHYS_MATH_H

#include "autotest.h"

class TestPhysMath : public QObject
{
    Q_OBJECT
private slots:
    void testCumNormal();
    void testInvCumNormal();
    void testInferNormal();
    void testInvNormalRange();

    void testSvp();
    void testSvpSlope();
};

DECLARE_TEST(TestPhysMath)

#endif
