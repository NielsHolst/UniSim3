/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_REG_EXP_H
#define TEST_REG_EXP_H

#include "autotest.h"

class TestRegExp : public QObject
{
    Q_OBJECT
private slots:
    void testMatchOne();
    void testMatchAll();
    void testMatchPairedApostrophes();
    void testNumberInParentheses();
};

DECLARE_TEST(TestRegExp)

#endif
