#ifndef TEST_BLIND_PORT_H
#define TEST_BLIND_PORT_H
#include "autotest.h"

class TestBlindPort : public QObject
{
    Q_OBJECT
private slots:
    void testFormat();
};

DECLARE_TEST(TestBlindPort)

#endif
