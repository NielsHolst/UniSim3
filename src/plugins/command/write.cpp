/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QDesktopServices>
#include <QFile>
#include <QUrl>
#include <QTextStream>
#include <base/box.h>
#include <base/command_help.h>
#include <base/computation.h>
#include <base/dialog_base.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "write.h"

using std::unique_ptr;
using namespace base;

namespace command {

PUBLISH(write)
HELP(write, "write", "writes box script")

write::write(QString name, Box *parent)
    : Command(name, parent), _option(BoxscriptConverter::Option::WriteUserScript), _doEdit(true)
{
}

write::~write() {
    Computation::changeStep(Computation::Step::Ready);
}

void write::doExecute() {
    _args.removeAt(0);
    extractArgs();
    writeFile();
    if (_doEdit)
        edit();
}

void write::extractArgs() {
    QString s = _args.join("");
    for (int i = 0; i < s.size(); ++i) {
        auto ch = s.at(i);
        if (ch == 'a')
            _option = BoxscriptConverter::Option::WriteAll;
        else if (ch == 'n')
            _doEdit = false;
        else
            ThrowException("Unknown write option. Only 'a' (all) and 'n' (no edit) are valid").value(qPrintable(ch));
    }
}

void write::writeFile() {
    Box *root =  Box::root();
    if (!root)
        dialog().error("No box script loaded");
    else {
        QFile file;
        environment().openOutputFile(file, ".box");
        QTextStream text(&file);
        BoxscriptConverter output(root, _option);
        text << output.toString();

        _filePath = environment().outputFilePath(".box");
        QString info("Box script written to '%1'");
        dialog().information(info.arg(_filePath));
        environment().incrementFileCounter();
    }
}

void write::edit() {
    QString url = "file:///" + _filePath;
    bool ok = QDesktopServices::openUrl(QUrl(url));
    if (!ok)
        ThrowException("Could not open editor");
}

}
