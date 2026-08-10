/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_NBD_H
#define TEST_NBD_H

#include "autotest.h"

class TestNbd : public QObject
{
    Q_OBJECT
private slots:
    void testProbability();
};

DECLARE_TEST(TestNbd)

#endif
