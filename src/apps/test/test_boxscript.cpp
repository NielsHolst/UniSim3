/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QFileInfo>
#include <base/box.h>
#include <base/command.h>
#include <base/dialog.h>
#include <base/exception.h>
#include "input_file_path.h"
#include "test_boxscript.h"

using namespace base;

void TestBoxScript::testPlotRPortsAsVector() {
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "test_box_script_ports_as_vector.box", 0);
    QCOMPARE(errors, dialog().errorCount());

    Box *sim = environment().root();
    Box *plot = sim->findOne<Box>("plot");
    QVector<QString> value = plot->port("ports")->value<QVector<QString>>();
    QCOMPARE(value, QVector<QString>() << "sim[step]");
}

void TestBoxScript::testPlotRPortsAsScalar() {
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "test_box_script_ports_as_scalar.box", 0);
    QCOMPARE(errors, dialog().errorCount());

    Box *sim = environment().root();
    Box *plot = sim->findOne<Box>("plot");
    QVector<QString> value = plot->port("ports")->value<QVector<QString>>();
    QCOMPARE(value, QVector<QString>() << "sim[step]");
}

void TestBoxScript::testPlotRPortsAsReference() {
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "test_box_script_ports_as_reference.box", 0);
    QCOMPARE(errors, dialog().errorCount());
}

void TestBoxScript::testStringInDoubleQuotes() {
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "box_script/string_in_double_quotes.box", 0);
    QCOMPARE(errors, dialog().errorCount());
}

void TestBoxScript::testStringInSingleQuotes() {
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "box_script/string_in_single_quotes.box", 0);
    QCOMPARE(errors, dialog().errorCount());
}

void TestBoxScript::testStringInFancyQuotes() {
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "box_script/string_in_fancy_quotes.box", 0);
    QCOMPARE(errors, dialog().errorCount());
}

void TestBoxScript::testNonAscii() {
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "box_script/non_ascii.box", 0);
    QCOMPARE(errors, dialog().errorCount());
}

void TestBoxScript::testPathWithSpaces() {
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << "box_script/path_with_spaces.box", 0);
    QVERIFY(errors < dialog().errorCount());
}

void TestBoxScript::testLoadAbsolutePath() {
    QString fileNamePath = inputFilePath("test_box_script_ports_as_vector.box");
    QFileInfo info(fileNamePath);
    int errors = dialog().errorCount();
    Command::submit(QStringList() << "load" << fileNamePath, 0);
    QCOMPARE(errors, dialog().errorCount());
}
