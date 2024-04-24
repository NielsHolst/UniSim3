#ifndef TEST_REG_EXP_H
#define TEST_REG_EXP_H

#include "autotest.h"

class TestRegExp : public QObject
{
    Q_OBJECT
private slots:
    void testMatchOne();
    void testMatchAll();
    void testMatchPairedApostrophes();
    void testNumberInParentheses();
};

DECLARE_TEST(TestRegExp)

#endif
