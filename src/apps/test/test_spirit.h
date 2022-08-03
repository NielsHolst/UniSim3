#ifndef TEST_SPIRIT_H
#define TEST_SPIRIT_H

#include "autotest.h"

class TestSpirit : public QObject
{
    Q_OBJECT
private slots:
    void test_name();
    void test_qualified_name();
    void test_summary();
    void test_object_name();
    void test_reference();
    void test_operand();
    void test_sign();
    void test_bare_date();
    void test_date();
    void test_time();
    void test_date_time();
    void test_bool();
    void test_quoted_string();
    void test_function_call();
    void test_operator();
    void test_operation();
    void test_expression();
    void test_grouped_expression();
    void test_skipper();
    void test_assignment();
    void test_box();
    void test_box_script();
};

DECLARE_TEST(TestSpirit)

#endif
