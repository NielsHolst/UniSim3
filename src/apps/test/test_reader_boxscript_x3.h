/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_READER_BOXSCRIPT_X3_H
#define TEST_READER_BOXSCRIPT_X3_H
#include "autotest.h"

class TestReaderBoxScriptX3 : public QObject
{
    Q_OBJECT
private slots:
    void testAuxAllTypes();
    void testConstructAmend();
    void testInitialize();
    void testInitializePortMissing();
    void testMultipleMatches();
    void testCondition();
    void testParentheses();
    void testDoubleError();
    void testOnOffByDate();
    void testMissingReference();
    void testMissingPlotTypes();
};

DECLARE_TEST(TestReaderBoxScriptX3)

#endif
