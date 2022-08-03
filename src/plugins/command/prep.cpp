/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QDesktopServices>
#include <QFile>
#include <QUrl>
#include <base/boxscript_preprocessor.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "prep.h"

using std::unique_ptr;
using namespace base;

namespace command {

PUBLISH(prep)
HELP(prep, "prep", "writes preprocessed box script")

prep::prep(QString name, Box *parent)
    : Command(name, parent)
{
}


void prep::doExecute() {
    preprocess();
}

QString prep::inputFilePath() {
    Environment &env(environment());
    QString fileName;
    switch (_args.size()) {
    case 1:
        fileName = env.latestLoadArg();
        break;
    case 2:
        fileName = _args.at(1);
        break;
    default:
        ThrowException("Too many arguments. Write 'prep <file-name>'.").
                hint("If the file name contains spaces then enclose it in apostrophes").value(_args.join(" "));
    }
    env.currentLoadArg(fileName);
    return environment().filePath(Environment::Input, fileName);
}

void prep::preprocess() {
    QString inputFilePath = prep::inputFilePath();
    BoxScriptPreprocessor preprocessor;
    QString output = preprocessor.preprocess(inputFilePath);
    environment().latestLoadArg(inputFilePath);

    QFile file;
    environment().openOutputFile(file, ".box");
    file.write(output.toUtf8());
    file.close();

    QString msg = "Preprocessed box script written to '%1'",
            filePath = environment().outputFilePath(".box");
    dialog().information(msg.arg(filePath));
    environment().incrementFileCounter();

    QString url = "file:///" + filePath;
    bool ok = QDesktopServices::openUrl(QUrl(url));
    if (!ok)
        ThrowException("Could not open editor");

}

}
