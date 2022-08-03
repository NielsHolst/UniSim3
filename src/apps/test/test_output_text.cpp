#include <iostream>
#include <base/box.h>
#include <base/box_builder.h>
#include "output_file.h"
#include "test_output_text.h"

using namespace base;
using namespace std;

void TestOutputText::testOk() {
    BoxBuilder builder;
    try {
        builder.
            box("Simulation").name("sim").
                port("steps").equals(5).
                box("Fibonacci").name("fibo").
                endbox().
                box("OutputText").name("text").
                    port("ports").equals("(sim[step] fibo[value])").
                endbox().
            endbox();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    Box *sim;
    try {
        sim = builder.root();
        sim->run();
    }
    catch(Exception &ex) {
        QString s = "Unexpected exception: " + ex.what();
        QFAIL(qPrintable(s));
    }
    OutputFile file;
    cout << "*** " << qPrintable(file.filePath()) << " "
              << qPrintable(file.columnLabels().join(" ")) << "\n";
    QCOMPARE(file.numRows(), 6);
    QStringList values = file.column("value"),
                expected;
    expected << "1" << "1" << "2" << "3" << "5" << "8";
    QCOMPARE(values, expected);
    delete sim;
}

