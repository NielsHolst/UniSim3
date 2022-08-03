#ifndef TEST_GENERAL_H
#define TEST_GENERAL_H

#include "autotest.h"

class TestGeneral : public QObject
{
    Q_OBJECT
private slots:
    void testEmpty();
    void testOne();
    void testTwo();
    void testOneApEmpty();
    void testOneApBlank();
    void testMixed();
};

DECLARE_TEST(TestGeneral)

#endif
