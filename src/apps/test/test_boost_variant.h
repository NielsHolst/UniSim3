#ifndef TEST_BOOST_VARIANT_H
#define TEST_BOOST_VARIANT_H

#include "autotest.h"

class TestBoostVariant : public QObject
{
    Q_OBJECT
private slots:
    void test_boost_variant();
    void test_derived_boost_variant();
    void test_x3_variant();
};

DECLARE_TEST(TestBoostVariant)

#endif
