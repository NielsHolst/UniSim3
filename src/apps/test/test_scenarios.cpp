#include <iostream>
#include <base/box.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/exception.h>
#include "input_file_path.h"
#include "output_file.h"
#include "test_scenarios.h"

using namespace base;

void TestScenarios::test3Lines() {
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "scenarios_3.box", this);
    QCOMPARE(dialog().errorCount(), errors);

    Command::submit(QStringList() << "run", this); // run resets errorCount
    QCOMPARE(dialog().errorCount(), 0);

    OutputFile output;
    std::cout << qPrintable(output.filePath()) << "\n";
}

