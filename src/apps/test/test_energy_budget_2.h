#ifndef TEST_ENERGY_BUDGET_2_H
#define TEST_ENERGY_BUDGET_2_H
#include "autotest.h"

class TestEnergyBudget2 : public QObject
{
    Q_OBJECT
private slots:
    void testSw();
    void testLw();
    void testConv();
    void testBudget2();
    void testBudget3();
    void testBudget4Advection();
    void testBudget5Plant();
    void testBudgetComplete();
};

DECLARE_TEST(TestEnergyBudget2)

#endif
