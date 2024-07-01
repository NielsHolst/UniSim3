/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_CIRCLE_BUFFER_H
#define TEST_CIRCLE_BUFFER_H

#include "autotest.h"

class TestCircleBuffer : public QObject
{
    Q_OBJECT
private slots:
    void testStdVersion();
    void testUniSimVersion();
};

DECLARE_TEST(TestCircleBuffer)

#endif
