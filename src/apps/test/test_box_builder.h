#ifndef TEST_BOX_BUILDER_H
#define TEST_BOX_BUILDER_H

#include "autotest.h"

namespace base {
    class  Box;
}

class TestBoxBuilder : public QObject
{
    Q_OBJECT
private slots:
    void testBuild1();
    void testBuild2();
};

DECLARE_TEST(TestBoxBuilder)

#endif
