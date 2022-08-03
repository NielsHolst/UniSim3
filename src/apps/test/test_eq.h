#ifndef TEST_EQ_H
#define TEST_EQ_H

#include "autotest.h"

class TestEq : public QObject
{
    Q_OBJECT
private slots:
    void testGompertz();
};

DECLARE_TEST(TestEq)

#endif
