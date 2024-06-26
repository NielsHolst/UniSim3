/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_SUM_H
#define TEST_SUM_H
#include <QFile>
#include "autotest.h"

class TestSum : public QObject
{
    Q_OBJECT
private slots:
    void testWithValues();
    void testWithoutValues();
};

DECLARE_TEST(TestSum)

#endif
