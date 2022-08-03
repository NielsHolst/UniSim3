#ifndef TEST_COMMAND_H
#define TEST_COMMAND_H

#include "autotest.h"

class TestCommand : public QObject
{
    Q_OBJECT
private slots:
    void testCreate();
    void testExecute();
};

DECLARE_TEST(TestCommand)

#endif
