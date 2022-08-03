#ifndef TEST_SIMULATION_H
#define TEST_SIMULATION_H

#include "autotest.h"

class TestSimulation : public QObject
{
    Q_OBJECT
private slots:
    void testDataFrame();
};

DECLARE_TEST(TestSimulation)

#endif
