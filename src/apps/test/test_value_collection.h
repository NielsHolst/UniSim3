/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_VALUE_COLLECTION_H
#define TEST_VALUE_COLLECTION_H

#include "autotest.h"

class TestValueCollection : public QObject
{
    Q_OBJECT
private slots:
    void testEmpty();
    void testOne();
    void testMany();
    void testUpdate();
    void testMixedTypes();
    void testMixedTypesToString();
};

DECLARE_TEST(TestValueCollection)

#endif
