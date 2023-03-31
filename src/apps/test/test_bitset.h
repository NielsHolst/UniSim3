#ifndef TEST_BITSET_H
#define TEST_BITSET_H
#include "autotest.h"

class TestBitset : public QObject
{
    Q_OBJECT
private slots:
    void testCombineAddresses();
};

DECLARE_TEST(TestBitset)

#endif
