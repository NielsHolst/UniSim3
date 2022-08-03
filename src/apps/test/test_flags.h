#ifndef TEST_FLAGS_H
#define TEST_FLAGS_H

#include "autotest.h"

class TestFlags : public QObject
{
    Q_OBJECT
private slots:
    void testColours();
    void testTastes();
};

DECLARE_TEST(TestFlags)

#endif
