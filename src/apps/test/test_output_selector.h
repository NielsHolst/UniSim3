/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_OUTPUT_SELECTOR_H
#define TEST_OUTPUT_SELECTOR_H
#include "autotest.h"

class TestOutputSelector : public QObject
{
    Q_OBJECT
private slots:
    void testPeriod1Iter();
    void testPeriodNIter();
    void testFinal1Iter();
    void testFinalNIter();
    void testPeriod1IterSkip();
    void testPeriodNIterSkip();
};

DECLARE_TEST(TestOutputSelector)

#endif
