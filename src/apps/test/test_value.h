/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_VALUE_H
#define TEST_VALUE_H

#include "autotest.h"

class TestValue : public QObject
{
    Q_OBJECT
private slots:
    void testBoolTyped();
    void testInitializeByValue();
    void testInitializeByPointer();
    void testAssignmentByValue();
    void testAssignmentByPointer();
    void testAssignmentTwice();
    void testAssignmentToUninitialized();
    void testBool();
    void testInt();
};

DECLARE_TEST(TestValue)

#endif
