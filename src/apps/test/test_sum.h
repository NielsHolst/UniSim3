#ifndef TEST_SUM_H
#define TEST_SUM_H
#include <QFile>
#include "autotest.h"

class TestSum : public QObject
{
    Q_OBJECT
private slots:
    void testWithValues();
    void testWithoutValues();
};

DECLARE_TEST(TestSum)

#endif
