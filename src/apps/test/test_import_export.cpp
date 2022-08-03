#include <iostream>
#include <base/box.h>
#include <base/exception.h>
#include <base/reader_boxscript.h>
#include "exception_expectation.h"
#include "input_file_path.h"
#include "output_file.h"
#include "test_import_export.h"

using namespace base;

inline auto str(QString s) { return qPrintable(s); }

inline QStringList paths(const QVector<Port*> &ports) {
    QStringList list;
    for (Port *port : ports)
        list << port->fullName();
    return list;
}

void TestImportExport::testImport() {
    bool excepted(false);
    BoxBuilder builder;
    Box *root;
    ReaderBoxScript reader(&builder);
    try {
        reader.parse(inputFilePath("box_script/import_export.box"));
        root = Box::root(builder.root());
        root->run();
    }
    UNEXPECTED_EXCEPTION;
    QVERIFY2(!root->port("hasError")->value<bool>(), str(root->port("errorMsg")->value<QString>()));

    OutputFile f(OutputFile::Format::WithFormatLine);
    QCOMPARE(f.numRows(), 4);

    Port *T = root->findOne<Port*>("sim/butterfly/time[T]");
    QVector<Port*> imports = T->importPorts();
    std::cout << str(paths(imports).join("\n")) << "\n";
}

void TestImportExport::testExport() {
}
