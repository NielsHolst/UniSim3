/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_RANDOM_H
#define TEST_RANDOM_H

#include "autotest.h"

class TestRandom : public QObject
{
    Q_OBJECT
private slots:
    void testDrawAt();
};

DECLARE_TEST(TestRandom)

#endif
