#ifndef TEST_UNIQUE_NAMES_H
#define TEST_UNIQUE_NAMES_H

#include "autotest.h"

class TestUniqueNames : public QObject
{
    Q_OBJECT
private slots:
    void testCombinations();
    void testValueName();
    void testUnnamedBox();
};

DECLARE_TEST(TestUniqueNames)

#endif
