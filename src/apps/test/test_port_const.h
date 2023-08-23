#ifndef TEST_PORT_CONST_H
#define TEST_PORT_CONST_H
#include "autotest.h"

class TestPortConst : public QObject
{
    Q_OBJECT
private slots:
    void testAllDefault();
//    void testAux();
//    void testExprRedefinesConst();
};

DECLARE_TEST(TestPortConst)

#endif
