#include <QFile>
#include <QStandardPaths>
#include <QStringList>
#include <base/copy_folder.h>
#include <base/exception.h>
#include "test_copy_folder.h"

using namespace base;

namespace {
    QDir temp() {
        QStringList paths = QStandardPaths::standardLocations(QStandardPaths::TempLocation);
        if (paths.isEmpty())
            ThrowException("Standard temporary folder not found");
        return QDir(paths.at(0));
    }

    QDir createFolder(QDir parent, QString folder) {
        bool ok = parent.mkdir(folder) && parent.cd(folder);
        if (!ok)
            ThrowException("Cannot create folder").value(folder);
        return parent;
    }

    bool deleteFolder(QDir parent, QString folder) {
        bool ok = parent.rmdir(folder);
        return ok;
    }

    bool createFile(QDir folder, QString fileName) {
        QFile file(folder.absoluteFilePath(fileName));
        bool ok = file.open(QIODevice::Text | QIODevice::WriteOnly);
        if (!ok)
            return false;
        // Write file name into the file
        file.write(qPrintable(fileName));
        return true;
    }

    QString readFile(QDir folder, QString path) {
        QString filePath = folder.absoluteFilePath(path);
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly|QIODevice::Text))
            ThrowException("Could not open file").value(filePath);
        return QString( file.readAll() );
    }

    bool deleteFile(QDir folder, QString path) {
        QFile file(folder.absoluteFilePath(path));
        bool ok = file.remove();
        return ok;
    }
}

void TestCopyFolder::testCreateFile() {
    QString fileName = "testCreateFile.txt",
            folderName = "testCreateFolder";
    QDir folder = createFolder(temp(), folderName);
    QVERIFY(createFile(folder, fileName));

    QCOMPARE(readFile(folder,fileName), QString(fileName));

    QVERIFY(deleteFile(folder, fileName));
    QVERIFY(deleteFolder(temp(), folderName));
}

void TestCopyFolder::testCopyFile() {
    QString fileName = "testCopyFile.txt";
    createFile(temp(), fileName);

    QDir destination = createFolder(temp(), "destination");
    copyFile(temp().absoluteFilePath(fileName),
             destination.absoluteFilePath(fileName));

    QCOMPARE(readFile(destination, fileName), fileName);

    QVERIFY(deleteFile(destination, fileName));
    QVERIFY(deleteFolder(temp(), "destination"));
    QVERIFY(deleteFile(temp(), fileName));
}

void TestCopyFolder::testBackupFolder1() {
//    QDir destination = createFolder(temp(), "destination");
//    QString fileName = "testCopyFolder.txt";
//    createFile(destination, fileName);

//    backupFolder(destination);
//    QVERIFY(temp().exists("destination_1/" + fileName));

//    QVERIFY(deleteFile(temp(), "destination/" + fileName));
//    QVERIFY(deleteFile(temp(), "destination_1/" + fileName));

//    QVERIFY(deleteFolder(temp(), "destination"));
//    QVERIFY(deleteFolder(temp(), "destination_1"));
}

void TestCopyFolder::testBackupFolder3() {
//    QDir destination = createFolder(temp(), "destination");
//    QString fileName = "testCopyFolder.txt";
//    createFile(destination, fileName);

//    createFolder(temp(), "destination_1");
//    createFolder(temp(), "destination_2");

//    backupFolder(destination);
//    QVERIFY(temp().exists("destination_3/" + fileName));

//    QVERIFY(deleteFile(temp(), "destination/" + fileName));
//    QVERIFY(deleteFile(temp(), "destination_3/" + fileName));

//    QVERIFY(deleteFolder(temp(), "destination"));
//    QVERIFY(deleteFolder(temp(), "destination_1"));
//    QVERIFY(deleteFolder(temp(), "destination_2"));
//    QVERIFY(deleteFolder(temp(), "destination_3"));
}

void TestCopyFolder::testCopyFolderHard() {
    QDir dir = temp();
    createFolder(dir, "source");
    dir.cd("source");
    createFile(dir, "a.txt");
    createFile(dir, "b.txt");
    createFolder(dir, "A");
    dir.cd("A");
    createFile(dir, "c.txt");

    QDir source(temp().absolutePath() + "/source"),
         destination(temp().absolutePath() + "/destination");
    copyFolderHard(source, destination);

    QVERIFY(destination.exists("a.txt"));
    QVERIFY(destination.exists("b.txt"));
    QVERIFY(destination.exists("A/c.txt"));

    QVERIFY(deleteFile(destination, "a.txt"));
    QVERIFY(deleteFile(destination, "b.txt"));
    QVERIFY(deleteFile(destination, "A/c.txt"));
    QVERIFY(deleteFolder(temp(), "destination/A"));
    QVERIFY(deleteFolder(temp(), "destination"));

    QVERIFY(deleteFile(source, "a.txt"));
    QVERIFY(deleteFile(source, "b.txt"));
    QVERIFY(deleteFile(source, "A/c.txt"));
    QVERIFY(deleteFolder(temp(), "source/A"));
    QVERIFY(deleteFolder(temp(), "source"));
}
