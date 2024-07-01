/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_BOX_H
#define TEST_BOX_H
#include "autotest.h"

class TestBox : public QObject
{
    Q_OBJECT
private slots:
    void testClone();
};

DECLARE_TEST(TestBox)

#endif
