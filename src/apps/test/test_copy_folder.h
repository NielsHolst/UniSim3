#ifndef TEST_COPY_FOLDER_H
#define TEST_COPY_FOLDER_H

#include "autotest.h"

class TestCopyFolder : public QObject
{
    Q_OBJECT
private slots:
    void testCreateFile();
    void testCopyFile();
    void testBackupFolder1();
    void testBackupFolder3();
    void testCopyFolderHard();
};

DECLARE_TEST(TestCopyFolder)

#endif
