/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <variant>
#include <base/value_collection.h>
#include "exception_expectation.h"
#include "test_value_collection.h"

using namespace base;

void TestValueCollection::testEmpty() {
    ValueCollection c;
    QVERIFY(c.isEmpty());
}

void TestValueCollection::testOne() {
    bool excepted(false);
    Value i(7);
    QVector<const Value*> ptrs;
    ptrs << &i;

    ValueCollection coll;
    try {
        coll.initialize(ptrs);
        coll.update();
    }
    UNEXPECTED_EXCEPTION;

    QCOMPARE(coll.type(), Value::Type::Int);
    vint val = coll.values().as<vint>();
    QCOMPARE(val.size(), 1);
    QCOMPARE(val.at(0), 7);
}


void TestValueCollection::testMany() {
    bool excepted(false);
    Value i(7), j(9), k(13);
    QVector<const Value*> ptrs;
    ptrs << &i << &j << &k;
    vint val;

    ValueCollection coll;
    try {
        coll.initialize(ptrs);
        coll.update();
        QCOMPARE(coll.type(), Value::Type::Int);
        val = coll.values().as<vint>();
        QCOMPARE(val.size(), 3);
        QCOMPARE(val, QVector<int>() << 7 << 9 << 13);
    }
    UNEXPECTED_EXCEPTION;

}

void TestValueCollection::testUpdate() {
    bool excepted(false);
    Value i(7), j(9), k(13);
    QVector<const Value*> ptrs;
    ptrs << &i << &j << &k;

    ValueCollection coll;
    try {
        coll.initialize(ptrs);
        coll.update();
    }
    UNEXPECTED_EXCEPTION;

    QCOMPARE(coll.type(), Value::Type::Int);
    vint val = coll.values().as<vint>();
    QCOMPARE(val.size(), 3);
    QCOMPARE(val, QVector<int>() << 7 << 9 << 13);

    i = 10; j = 20; k = 30;
    val = coll.values().as<vint>();
    QCOMPARE(val, QVector<int>() << 7 << 9 << 13);

    coll.update();
    val = coll.values().as<vint>();
    QCOMPARE(val, QVector<int>() << 10 << 20 << 30);

}

void TestValueCollection::testMixedTypes() {
    bool excepted(false);
    bool   i = true;
    int    j = 13;
    double k = 3.12;
    Value  u = &i, v = &j, w = &k;
    QCOMPARE(u.type(), Value::Type::Bool);
    QCOMPARE(v.type(), Value::Type::Int);
    QCOMPARE(w.type(), Value::Type::Double);

    QVector<const Value*> ptrs;
    ptrs << &u << &v << &w;

    ValueCollection coll;
    try {
        coll.initialize(ptrs);
        coll.update();
    }
    UNEXPECTED_EXCEPTION;

    QCOMPARE(coll.type(), Value::Type::Double);
    vdouble val = coll.values().as<vdouble>();
    QCOMPARE(val.size(), 3);
    QCOMPARE(val, QVector<double>() << 1. << 13. << 3.12);

    i = false; j = 20; k = 33.3;
    val = coll.values().as<vdouble>();
    QCOMPARE(val, QVector<double>() << 1. << 13. << 3.12);

    coll.update();
    val = coll.values().as<vdouble>();
    QCOMPARE(val, QVector<double>() << 0. << 20. << 33.3);
}

void TestValueCollection::testMixedTypesToString() {
    bool excepted(false);
    int    i = 13;
    QDate  j = QDate(2021,12,24);
    Value  u = &i, v = &j;
    QCOMPARE(u.type(), Value::Type::Int);
    QCOMPARE(v.type(), Value::Type::Date);

    QVector<const Value*> ptrs;
    ptrs << &u << &v;

    ValueCollection coll;
    try {
        coll.initialize(ptrs);
        coll.update();
    }
    UNEXPECTED_EXCEPTION;

    QCOMPARE(coll.type(), Value::Type::String);
    vQString val = coll.values().as<vQString>();
    QCOMPARE(val.size(), 2);
    QCOMPARE(val, vQString() << "13" << "2021/12/24");

    i = 10; j = QDate(2021,12,31);
    val = coll.values().as<vQString>();
    QCOMPARE(val, vQString() << "13" << "2021/12/24");

    coll.update();
    val = coll.values().as<vQString>();
    QCOMPARE(val, vQString() << "10" << "2021/12/31");
}
