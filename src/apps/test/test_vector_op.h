/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef TEST_VECTOR_OP_H
#define TEST_VECTOR_OP_H

#include "autotest.h"

class TestVectorOp : public QObject
{
    Q_OBJECT
private slots:
    void testPlusVecVec();
    void testPlusVecScalar();
    void testPlusVecVecVec();
    void testPlusVecVecScalar();

    void testMinusVecVec();
    void testMinusVecScalar();
    void testMinusVecVecVec();
    void testMinusVecVecScalar();

    void testMultiplyVecVec();
    void testMultiplyVecScalar();
    void testMultiplyVecVecVec();
    void testMultiplyVecVecScalar();

    void testSum();
    void testWeightedAverage();
    void testSumOfProducts();
};

DECLARE_TEST(TestVectorOp)

#endif
