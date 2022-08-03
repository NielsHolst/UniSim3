#ifndef TEST_NODE_H
#define TEST_NODE_H
#include "autotest.h"

class TestNode : public QObject
{
    Q_OBJECT
private slots:
    void testClassName();
};

DECLARE_TEST(TestNode)

#endif
