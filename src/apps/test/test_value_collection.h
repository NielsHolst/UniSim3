#ifndef TEST_VALUE_COLLECTION_H
#define TEST_VALUE_COLLECTION_H

#include "autotest.h"

class TestValueCollection : public QObject
{
    Q_OBJECT
private slots:
    void testEmpty();
    void testOne();
    void testMany();
    void testUpdate();
    void testMixedTypes();
    void testMixedTypesToString();
};

DECLARE_TEST(TestValueCollection)

#endif
