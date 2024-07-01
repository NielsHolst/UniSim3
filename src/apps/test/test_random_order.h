/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_RANDOM_ORDER_H
#define TEST_RANDOM_ORDER_H

#include "autotest.h"

class TestRandomOrder : public QObject
{
    Q_OBJECT
private slots:
    void test3();
};

DECLARE_TEST(TestRandomOrder)

#endif
