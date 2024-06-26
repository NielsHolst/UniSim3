/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_UNISIM_CALENDAR_H
#define TEST_UNISIM_CALENDAR_H

#include <QDate>
#include <QList>
#include "autotest.h"

namespace base {
    class Box;
}

class TestCalendar : public QObject
{
    Q_OBJECT
private slots:
	void initTestCase();
    void init();
    void cleanup();

    void testSolarElevation();
    void testDayLength();
    void testDaySteps();
    void testHourSteps();
    void testAngot();

private:
    QDate finalCalendarDate();

    struct Location{
        Location(QString nam, double lat, double lon) :
            name(nam), latitude(lat), longitude(lon) {}
        QString name;
        double latitude, longitude;
    };

    QList<Location> locations;
    QList<QDateTime> dates;
    QList<double> hours;

    double solarNoonDiff[4][4];
    QTime dayLength[4][4];
    double solarElev[4][4][3];

    base::Box *sim, *calendar, *sun;
};

DECLARE_TEST(TestCalendar)


#endif
