/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_DATA_FRAME_H
#define TEST_DATA_FRAME_H

#include "autotest.h"

class TestDataFrame : public QObject
{
    Q_OBJECT
private slots:
    void testDimensions();

    void testContentBothLabelled();
    void testContentColumnLabelled();
    void testContentRowLabelled();
    void testContentNoLabelling();

    void testAsInt();
};

DECLARE_TEST(TestDataFrame)

#endif
