#ifndef TEST_MAKER_H
#define TEST_MAKER_H

#include "autotest.h"

class TestMaker : public QObject
{
    Q_OBJECT
private slots:
    void testReplicates();
    void testNames();
};

DECLARE_TEST(TestMaker)

#endif
