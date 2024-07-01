/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_PORT_STATUS_H
#define TEST_PORT_STATUS_H
#include "autotest.h"

class TestPortStatus : public QObject
{
    Q_OBJECT
private slots:
    void testAllDefault();
    void testAuxDefinesExpr();
    void testAuxDefinesValue();
    void testExprRedefinesExpr();
    void testExprRedefinesValue();
    void testValueRedefinesExpr();
    void testValueRedefinesValue();
    void testValueRedefinesDefault();
};

DECLARE_TEST(TestPortStatus)

#endif
