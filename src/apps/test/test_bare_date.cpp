#include <QDate>
#include <base/bare_date.h>
#include "test_bare_date.h"

using namespace base;

void TestBareDate::testComparisonsWithBareDate() {
    BareDate current(5, 15);

    QVERIFY(current <  BareDate(12,12));
    QVERIFY(current <= BareDate(12,12));
    QVERIFY(current <= BareDate( 5,15));
    QVERIFY(current == BareDate( 5,15));
    QVERIFY(current != BareDate( 4,15));
    QVERIFY(current != BareDate( 5,16));
    QVERIFY(current >  BareDate( 3, 3));
    QVERIFY(current >= BareDate( 5,15));
    QVERIFY(current >= BareDate( 3, 3));
}

void TestBareDate::testComparisonsWithQDate() {
    QDate current(2017, 5, 15);

    QVERIFY(         current <  BareDate(12,12));
    QVERIFY(         current <= BareDate(12,12));
    QVERIFY(         current <= BareDate( 5,15));
    QVERIFY(         current == BareDate( 5,15));
    QVERIFY(         current != BareDate( 4,15));
    QVERIFY(         current != BareDate( 5,16));
    QVERIFY(         current >  BareDate( 3, 3));
    QVERIFY(         current >= BareDate( 5,15));
    QVERIFY(         current >= BareDate( 3, 3));

    QVERIFY(BareDate(12,12)  >  current);
    QVERIFY(BareDate(12,12)  >= current);
    QVERIFY(BareDate( 5,15)  >= current);
    QVERIFY(BareDate( 5,15)  == current);
    QVERIFY(BareDate( 4,15)  != current);
    QVERIFY(BareDate( 5,16)  != current);
    QVERIFY(BareDate( 3, 3)  <  current);
    QVERIFY(BareDate( 5,15)  <= current);
    QVERIFY(BareDate( 3, 3)  <= current);
}
