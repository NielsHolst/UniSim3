/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <iostream>
#include <base/box.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/exception.h>
#include "exception_expectation.h"
#include "output_file.h"
#include "test_blind_port.h"

using namespace base;

void TestBlindPort::testFormat() {
    bool excepted(false);
    try {
        Command::submit(QStringList() << "run" << "blind-port-format.box", nullptr);
        environment().root();
    }
    UNEXPECTED
    QCOMPARE(dialog().errorCount(), 0);

    OutputFile file;
    std::cout << "*** " << qPrintable(file.filePath()) << "\n"
              << qPrintable(file.columnLabels().join(" - ")) << "\n"
              << qPrintable(file.columnFormats().join(" - ")) << "\n";
}
