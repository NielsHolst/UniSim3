#include <iostream>
#include <QCoreApplication>
#include <QDir>
#include <base/dialog_stub.h>
#include <base/environment.h>
#include <base/organisation.h>
#include "autotest.h"

namespace {

QDir saveDirInput, saveDirOutput;

QString expectedInputPath() {
    QString path = QCoreApplication::applicationDirPath();
    QDir dir = QDir(path);
    dir.cdUp();
    dir.cd("src/apps/test/input");
    return dir.absolutePath();
}

QString expectedOutputPath() {
    QString path = QCoreApplication::applicationDirPath();
    QDir dir = QDir(path);
    dir.cdUp();
    dir.cd("output");
    return dir.absolutePath();
}

bool setInputPath() {
    saveDirInput = base::environment().dir(base::Environment::Input);
    QDir dir = QDir();
    bool ok = dir.cd(expectedInputPath());
    if (ok)
        base::environment().dir(base::Environment::Input, dir);
    else
        std::cout << "Input path not found: " << qPrintable(expectedInputPath());
    return ok;
}

bool setOutputPath() {
    saveDirOutput = base::environment().dir(base::Environment::Output);
    QDir dir = QDir();
    bool ok = dir.cd(expectedOutputPath());
    if (ok)
        base::environment().dir(base::Environment::Output, dir);
    else
        std::cout << "Output path not found: " << qPrintable(expectedOutputPath());
    return ok;
}

}


int main(int argc, char *argv[])
{
    // Init app
    int result = EXIT_FAILURE;
    QApplication app(argc, argv);

    // Set up dialog stub
    new base::DialogStub(qApp);

    // Set input and output folders
    if (setInputPath() && setOutputPath()) {

        // Manipulate latest load argument
        base::environment().latestLoadArg("test.box");

        // Run test
        result = AutoTest::run(argc, argv);
        QString msg = (result==0) ? "All tests passed" : "One or more tests failed";
        std::cout << "\n\n" << qPrintable(msg) << "\n";
    }

    // Re-establish input and output folders
    base::environment().dir(base::Environment::Input,  saveDirInput);
    base::environment().dir(base::Environment::Output, saveDirOutput);

    // Done
    return result;
}
