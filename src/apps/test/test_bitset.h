/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_BITSET_H
#define TEST_BITSET_H
#include "autotest.h"

class TestBitset : public QObject
{
    Q_OBJECT
private slots:
    void testCombineAddresses();
};

DECLARE_TEST(TestBitset)

#endif
