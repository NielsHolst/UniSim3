#ifndef TEST_REFERENCES_H
#define TEST_REFERENCES_H
#include "autotest.h"

class TestReferences : public QObject
{
    Q_OBJECT
private slots:
    void testForward();
    void testUpdate();
    void testDots();
    void testEmptyVector();
};

DECLARE_TEST(TestReferences)

#endif
