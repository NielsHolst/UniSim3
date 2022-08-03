#ifndef TEST_PATH_H
#define TEST_PATH_H

#include "autotest.h"

class TestPath : public QObject
{
    Q_OBJECT
private slots:
    void testFindBox();
    void testFindClassName();
};

DECLARE_TEST(TestPath)

#endif
