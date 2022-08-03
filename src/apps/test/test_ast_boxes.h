#ifndef TEST_AST_BOXES_H
#define TEST_AST_BOXES_H

#include "autotest.h"

namespace base  {
    class BoxBuilder;
    class BoxReaderBase;
}
class TestAstBoxes : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void testSimple();
    void testComments();
    void testWhitespaceInVector();
    void testWhitespaceInPath();
    void testTransform();
private:
    base::BoxBuilder *builder;
    base::BoxReaderBase *reader;
};

DECLARE_TEST(TestAstBoxes)

#endif
