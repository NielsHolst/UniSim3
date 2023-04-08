#ifndef TEST_PORT_STATUS_H
#define TEST_PORT_STATUS_H
#include "autotest.h"

class TestPortStatus : public QObject
{
    Q_OBJECT
private slots:
    void testAllDefault();
    void testAuxDefinesExpr();
    void testAuxDefinesValue();
    void testExprRedefinesExpr();
    void testExprRedefinesValue();
    void testValueRedefinesExpr();
    void testValueRedefinesValue();
    void testValueRedefinesDefault();
};

DECLARE_TEST(TestPortStatus)

#endif
