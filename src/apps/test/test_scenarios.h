#ifndef TEST_SCENARIOS_H
#define TEST_SCENARIOS_H
#include <QFile>
#include "autotest.h"

class TestScenarios : public QObject
{
    Q_OBJECT
private slots:
    void test3Lines();
private:
    QFile _file;
};

DECLARE_TEST(TestScenarios)

#endif
