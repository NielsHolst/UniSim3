/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_SIMULATION_H
#define TEST_SIMULATION_H
#include "autotest.h"

class TestSimulation : public QObject
{
    Q_OBJECT
private slots:
    void testGrowth();
    void testSteps();
    void testCalendarSteps();
    void testCalendarPeriodDateDayStep();
    void testCalendarPeriodDateHourStep();
    void testCalendarPeriodDateTime();
    void testOutputSelectorBeginStep();
};

DECLARE_TEST(TestSimulation)

#endif
