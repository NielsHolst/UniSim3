#ifndef TEST_HISTORY_H
#define TEST_HISTORY_H

#include "autotest.h"

class TestHistory : public QObject
{
    Q_OBJECT
private slots:
    void testAddOne();
    void testOverFullHistoryPrevious();
    void testNotFullHistoryPrevious();
    void testJustFullHistoryPrevious();

    void testOverFullHistoryNext();
    void testNotFullHistoryNext();
    void testJustFullHistoryNext();
};

DECLARE_TEST(TestHistory)

#endif
