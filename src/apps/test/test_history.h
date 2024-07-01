/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_HISTORY_H
#define TEST_HISTORY_H

#include "autotest.h"

class TestHistory : public QObject
{
    Q_OBJECT
private slots:
    void testAddOne();
    void testOverFullHistoryPrevious();
    void testNotFullHistoryPrevious();
    void testJustFullHistoryPrevious();

    void testOverFullHistoryNext();
    void testNotFullHistoryNext();
    void testJustFullHistoryNext();
};

DECLARE_TEST(TestHistory)

#endif
