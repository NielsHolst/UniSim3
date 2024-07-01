/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef EXCEPTION_EXPECTATION_H
#define EXCEPTION_EXPECTATION_H
#include <exception>
#include <iostream>
#include <qtestcase.h>
#include <base/box.h>
#include <base/exception.h>

using namespace base;

#define UNEXPECTED_EXCEPTION \
catch(const base::Exception &ex) { \
    QString s = "Unexpected exception: " + ex.what(); \
    QFAIL(qPrintable(s)); \
} \
catch (const std::exception& ex) { \
    QString s = "Unexpected exception: " + QString(ex.what()); \
    QFAIL(qPrintable(s)); \
} \
catch (...) { \
    QFAIL("Unexpected exception"); \
} \
QVERIFY(!excepted); \
excepted = false

#define EXPECTED_EXCEPTION \
catch(...) { \
    excepted = true; \
} \
QVERIFY(excepted); \
excepted = false

#define EXPECTED_EXCEPTION_SHOWN \
catch(const Exception &ex) { \
    QString s = "Expected exception:\n" + ex.what(); \
    std::cout << qPrintable(s) << std::endl; \
    excepted = true; \
} \
catch(...) { \
    excepted = true; \
} \
QVERIFY(excepted); \
excepted = false

inline bool noRuntimeError(base::Box *sim) {
    if (sim->port("hasError")->value<bool>()) {
        std::cout << qPrintable(sim->port("errorMsg")->value<QString>()) << std::endl;
        return false;
    }
    return true;
}

#endif // EXCEPTION_EXPECTATION_H
