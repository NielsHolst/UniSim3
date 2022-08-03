#ifndef TEST_IMPORT_EXPORT_H
#define TEST_IMPORT_EXPORT_H
#include "autotest.h"

class TestImportExport : public QObject
{
    Q_OBJECT
private slots:
    void testImport();
    void testExport();
};

DECLARE_TEST(TestImportExport)

#endif
