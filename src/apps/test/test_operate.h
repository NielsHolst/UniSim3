/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_EXPRESSION_OPERATION_H
#define TEST_EXPRESSION_OPERATION_H

#include "autotest.h"

class TestOperate : public QObject
{
    Q_OBJECT
private slots:
    void testAdd();
    void testAddNumbers();
    void testAddDate();
    void testAddTime();
    void testAddDateTime();
    void testError();
    void testVector();
};

DECLARE_TEST(TestOperate)

#endif
