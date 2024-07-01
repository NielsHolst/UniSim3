/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_EXPRESSION_PARSER_H
#define TEST_EXPRESSION_PARSER_H

#include "autotest.h"

namespace base {
    class  Box;
}

class TestExpressionParser : public QObject
{
    Q_OBJECT
private slots:
    void testAstHandling();
    void testOperation();
    void testPathWithPort();
    void testPathWithoutPort();
    void testError();
};

DECLARE_TEST(TestExpressionParser)

#endif
