#ifndef TEST_SAVE_GRAMMAR_H
#define TEST_SAVE_GRAMMAR_H

#include "autotest.h"

class TestSaveGrammar : public QObject
{
    Q_OBJECT
private slots:
    void testExecute();
};

DECLARE_TEST(TestSaveGrammar)

#endif
