/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_FLAGS_H
#define TEST_FLAGS_H

#include "autotest.h"

class TestFlags : public QObject
{
    Q_OBJECT
private slots:
    void testColours();
    void testTastes();
};

DECLARE_TEST(TestFlags)

#endif
