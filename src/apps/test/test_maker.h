/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_MAKER_H
#define TEST_MAKER_H

#include "autotest.h"

class TestMaker : public QObject
{
    Q_OBJECT
private slots:
    void testReplicates();
    void testNames();
};

DECLARE_TEST(TestMaker)

#endif
