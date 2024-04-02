#ifndef TEST_GRADIENT_ASCENT_H
#define TEST_GRADIENT_ASCENT_H
#include "autotest.h"

class TestGradientAscent : public QObject
{
    Q_OBJECT
private slots:
    void testParabolic();
};

DECLARE_TEST(TestGradientAscent)

#endif
