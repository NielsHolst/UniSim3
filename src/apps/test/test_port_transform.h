#ifndef TEST_PORT_TRANSFORM_H
#define TEST_PORT_TRANSFORM_H
#include <QFile>
#include "autotest.h"

class TestPortTransform : public QObject
{
    Q_OBJECT
private slots:
    void testSum();
    void testSumEmptySet();
private:
    QFile _file;
};

DECLARE_TEST(TestPortTransform)

#endif
