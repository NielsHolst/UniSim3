/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_EXPRESSION_H
#define TEST_EXPRESSION_H

#include "autotest.h"

class TestExpression : public QObject
{
    Q_OBJECT
private slots:
    void testPush();
    void testToPostfix();
    void testEmpty();
    void testSingle();
    void testAddition();
    void testResultType();
    void testNegation();
    void testExponentiation();
    void testFunctionCall();
    void testParentheses();
    void textBoxScriptExpression();
};

DECLARE_TEST(TestExpression)

#endif
