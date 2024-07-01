/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_BOOST_VARIANT_H
#define TEST_BOOST_VARIANT_H

#include "autotest.h"

class TestBoostVariant : public QObject
{
    Q_OBJECT
private slots:
    void test_boost_variant();
    void test_derived_boost_variant();
    void test_x3_variant();
};

DECLARE_TEST(TestBoostVariant)

#endif
