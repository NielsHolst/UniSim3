#include <iostream>
#include <QDir>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/test_num.h>
#include <base/mega_factory.h>
#include <base/phys_math.h>
#include "test_calendar.h"

using std::cout;
using namespace base;
using phys_math::PI;

const QDate WEATHER_DATE = QDate(2009, 9, 1);

/*
NOAA Solar Position Calculator
[https://www.esrl.noaa.gov/gmd/grad/solcalc/]

Boston:			+42 32" Lat +71 03" Lon (+42.533, +71.050)
Buenos Aires:	-34 36" Lat +58 27" Lon (-34.600, +58.450)
Cape Town: 		-33 55" Lat -18 22" Lon (-33.917, -18.367)
Helsinki:		+60 10"	Lat -24 58" Lon (+60.167, -24.967)

Solar noon
11:53:40
11:44:12
11:50:11
11:29:57
*/

void TestCalendar::initTestCase() {
    // Check locations in all four quadrants:
    locations.append(Location("Boston",         +42.533, -71.050));
    locations.append(Location("Buenos Aires",   -34.600, -58.450));
    locations.append(Location("Cape Town",      -33.917, +18.367));
    locations.append(Location("Helsinki",       +60.167, +24.967));

    // Check dates of all seasons
    dates.append(QDate(2010, 1,15).startOfDay());
    dates.append(QDate(2010, 4,15).startOfDay());
    dates.append(QDate(2010, 7,15).startOfDay());
    dates.append(QDate(2010,10,15).startOfDay());

    // Check night, morning and afternoon
    hours.append(1.);
    hours.append(9.);
    hours.append(16.);

    QTime noon(12,0,0);
    solarNoonDiff[0][0] = noon.secsTo(QTime(11,53,40))/60./60.;
    solarNoonDiff[0][1] = noon.secsTo(QTime(11,44,12))/60./60.;
    solarNoonDiff[0][2] = noon.secsTo(QTime(11,50,11))/60./60.;
    solarNoonDiff[0][3] = noon.secsTo(QTime(11,29,57))/60./60.;

    solarNoonDiff[1][0] = noon.secsTo(QTime(13,06,16))/60./60.;
    solarNoonDiff[1][1] = noon.secsTo(QTime(12,53,49))/60./60.;
    solarNoonDiff[1][2] = noon.secsTo(QTime(12,59,47))/60./60.;
    solarNoonDiff[1][3] = noon.secsTo(QTime(12,39,33))/60./60.;

    solarNoonDiff[2][0] = noon.secsTo(QTime(12,55,54))/60./60.;
    solarNoonDiff[2][1] = noon.secsTo(QTime(12,46,35))/60./60.;
    solarNoonDiff[2][2] = noon.secsTo(QTime(12,52,29))/60./60.;
    solarNoonDiff[2][3] = noon.secsTo(QTime(12,32,19))/60./60.;

    solarNoonDiff[3][0] = noon.secsTo(QTime(12,29,30))/60./60.;
    solarNoonDiff[3][1] = noon.secsTo(QTime(12,20,11))/60./60.;
    solarNoonDiff[3][2] = noon.secsTo(QTime(12,26,05))/60./60.;
    solarNoonDiff[3][3] = noon.secsTo(QTime(12,05,55))/60./60.;

    dayLength[0][0] = QTime( 9, 26,15);
    dayLength[0][1] = QTime(13, 22, 16);
    dayLength[0][2] = QTime(14, 58,14);
    dayLength[0][3] = QTime(11,  5, 55);
    dayLength[1][0] = QTime(14, 11,  8);
    dayLength[1][1] = QTime(11, 13, 24);
    dayLength[1][2] = QTime(10,  4, 21);
    dayLength[1][3] = QTime(12, 55, 42);
    dayLength[2][0] = QTime(14,  9, 24);
    dayLength[2][1] = QTime(11, 14, 38);
    dayLength[2][2] = QTime(10,  6,  4);
    dayLength[2][3] = QTime(12, 54, 29);
    dayLength[3][0] = QTime( 6, 40, 46);
    dayLength[3][1] = QTime(14, 34, 27);
    dayLength[3][2] = QTime(18,  7, 24);
    dayLength[3][3] = QTime(10, 12, 14);

    solarElev[0][0][0] = 0;
    solarElev[0][0][1] = 15.05;
    solarElev[0][0][2] = 5.14;
    solarElev[0][1][0] = 0;
    solarElev[0][1][1] = 41.65;
    solarElev[0][1][2] = 25.88;
    solarElev[0][2][0] = 0;
    solarElev[0][2][1] = 48.89;
    solarElev[0][2][2] = 34.38;
    solarElev[0][3][0] = 0;
    solarElev[0][3][1] = 28.64;
    solarElev[0][3][2] = 10.29;

    solarElev[1][0][0] = 0;
    solarElev[1][0][1] = 35.40;
    solarElev[1][0][2] = 49.02;
    solarElev[1][1][0] = 0;
    solarElev[1][1][1] = 19.15;
    solarElev[1][1][2] = 27.39;
    solarElev[1][2][0] = 0;
    solarElev[1][2][1] = 10.19;
    solarElev[1][2][2] = 19.53;
    solarElev[1][3][0] = 0;
    solarElev[1][3][1] = 33.58;
    solarElev[1][3][2] = 37.42;

    solarElev[2][0][0] = 0;
    solarElev[2][0][1] = 36.90;
    solarElev[2][0][2] = 47.61;
    solarElev[2][1][0] = 0;
    solarElev[2][1][1] = 20.83;
    solarElev[2][1][2] = 26.62;
    solarElev[2][2][0] = 0;
    solarElev[2][2][1] = 11.78;
    solarElev[2][2][2] = 18.92;
    solarElev[2][3][0] = 0;
    solarElev[2][3][1] = 35.14;
    solarElev[2][3][2] = 36.14;

    solarElev[3][0][0] = 0;
    solarElev[3][0][1] = 0;
    solarElev[3][0][2] = 0;
    solarElev[3][1][0] = 0;
    solarElev[3][1][1] = 27.53;
    solarElev[3][1][2] = 25.49;
    solarElev[3][2][0] = 0;
    solarElev[3][2][1] = 37.34;
    solarElev[3][2][2] = 36.40;
    solarElev[3][3][0] = 0;
    solarElev[3][3][1] = 12.22;
    solarElev[3][3][2] = 7.41;
}

void TestCalendar::init() {
    BoxBuilder builder;
    builder.
    box("Simulation").name("sim").
        box("Calendar").name("calendar").
        endbox().
        box("Sun").name("sun").
        endbox().
    endbox();
    sim = builder.root();
    calendar = sim->findOne<Box>("calendar");
    sun = sim->findOne<Box>("sun");
}

void TestCalendar::cleanup() {
    delete sim;
}

void TestCalendar::testSolarElevation() {
    try {
        for (int lo = 0; lo < locations.size(); ++lo) {
            calendar->port("latitude")->equals(locations[lo].latitude);
            for (int da = 0; da < dates.size(); ++da) {
                for (int ho = 0; ho < hours.size(); ++ho) {
                    double astroHour = hours[ho] - solarNoonDiff[lo][da];
                    int astroSecs = int(astroHour*3600);
                    QTime initialTime = QTime(0,0,0).addSecs(astroSecs);
                    QDateTime initialDateTime = QDateTime(dates[da].date(), initialTime);
                    calendar->port("initialDateTime")->equals(initialDateTime);
                    sim->initializeFamily();
                    sim->resetFamily();
                    double sinb = sun->port("sinb")->value<double>();
                    double estSolarElev = asin(sinb)/PI*180.;
    //                cout << lo << " "
    //                    << da << " "
    //                    << ho << " "
    //                    << estSolarElev << " "
    //                    << solarElev[lo][da][ho] << "\n";
                    QVERIFY(fabs(estSolarElev - solarElev[lo][da][ho]) < 1.);
                }
            }
        }
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}


void TestCalendar::testDayLength() {
    try {
        for (int lo = 0; lo < locations.size(); ++lo) {
            calendar->port("latitude")->equals(locations[lo].latitude);
            calendar->port("initialDateTime")->equals(QDate(2010, 1, 1));
            sim->initializeFamily();
            sim->resetFamily();
            for (int da = 0; da < dates.size(); ++da) {
                while (calendar->port("date")->value<QDate>() < dates[da].date()) {
                    sim->updateFamily();
                }
            double trueDayLength = QTime(0,0,0).secsTo(dayLength[lo][da])/60./60.;
            double estDayLength = sun->port("dayLength")->value<double>();
    //        cout << "Day length deviation (h)" << lo << ": " << (estDayLength - trueDayLength) << "\n";
            QVERIFY(fabs(estDayLength - trueDayLength) < 0.5);
            }
        }
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestCalendar::testAngot() {
    try {
        calendar->port("latitude")->equals(50);
        calendar->port("initialDateTime")->equals(QDateTime(QDate(2001,1,30), QTime(14,0,0)));
        sim->initializeFamily();
        sim->resetFamily();
        double angot = sun->port("angot")->value<double>();
        cout << angot << "\n";
        QVERIFY(TestNum::eq(angot, 414.2890));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}


void TestCalendar::testDaySteps() {
    try {
        QDate initialDate = calendar->port("initialDateTime")->value<QDateTime>().date();
        sim->initializeFamily();
        sim->resetFamily();

        const int n = 7;
        for (int i = 0; i < n; ++i){
            sim->updateFamily();
        }
        QCOMPARE(initialDate.addDays(n), calendar->port("date")->value<QDate>());
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

void TestCalendar::testHourSteps() {
    try {
        QDateTime initialDateTime = QDateTime(QDate(2000,1,1),QTime(12,0));
        calendar->port("timeUnit")->equals('h');
        calendar->port("timeStep")->equals(4);
        calendar->port("initialDateTime")->equals(initialDateTime);
        sim->initializeFamily();
        sim->resetFamily();

        const int n = 8;
        for (int i = 0; i < n; ++i){
            sim->updateFamily();
        }
        //Expected: 12h + 8*4h = 44h = 1d 20h
        QCOMPARE(calendar->port("date")->value<QDate>(), initialDateTime.addDays(1).date());
        QCOMPARE(calendar->port("time")->value<QTime>(), QTime(20,0));
    }
    catch (Exception &ex) {
        QFAIL(qPrintable(ex.what()));
    }
}

