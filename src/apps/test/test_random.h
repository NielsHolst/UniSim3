#ifndef TEST_RANDOM_H
#define TEST_RANDOM_H

#include "autotest.h"

class TestRandom : public QObject
{
    Q_OBJECT
private slots:
    void testDrawAt();
};

DECLARE_TEST(TestRandom)

#endif
