#ifndef TEST_PORT_BUFFER_H
#define TEST_PORT_BUFFER_H

#include "autotest.h"
namespace base {
class Box;
}

class TestPortBuffer : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();

    void testCreateBool();

    void testImportNoBuffer();
    void testImportFromBuffer();
    void testImportToBuffer();
    void testImportBufferToBuffer();

    void testImportVectorNoBuffer();
    void testImportVectorFromBuffer();
    void testImportVectorToBuffer();
    void testImportVectorBufferToBuffer();
private:
    base::Box *sim;
};

DECLARE_TEST(TestPortBuffer)

#endif
