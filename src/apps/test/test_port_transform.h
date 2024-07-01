/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_PORT_TRANSFORM_H
#define TEST_PORT_TRANSFORM_H
#include <QFile>
#include "autotest.h"

class TestPortTransform : public QObject
{
    Q_OBJECT
private slots:
    void testSum();
    void testSumEmptySet();
private:
    QFile _file;
};

DECLARE_TEST(TestPortTransform)

#endif
