/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_GENERAL_H
#define TEST_GENERAL_H

#include "autotest.h"

class TestGeneral : public QObject
{
    Q_OBJECT
private slots:
    void testEmpty();
    void testOne();
    void testTwo();
    void testOneApEmpty();
    void testOneApBlank();
    void testMixed();
};

DECLARE_TEST(TestGeneral)

#endif
