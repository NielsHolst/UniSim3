/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_CONVERT_TIME_H
#define TEST_CONVERT_TIME_H

#include "autotest.h"

class TestConvertTime : public QObject
{
    Q_OBJECT
private slots:
    void testFromString();
};

DECLARE_TEST(TestConvertTime)

#endif
