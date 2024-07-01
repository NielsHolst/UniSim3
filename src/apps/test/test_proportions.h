/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_PROPORTIONS_H
#define TEST_PROPORTIONS_H

#include "autotest.h"

class TestProportions : public QObject
{
    Q_OBJECT
private slots:
    void test1();
    void test2();
    void test3();
};

DECLARE_TEST(TestProportions)

#endif
