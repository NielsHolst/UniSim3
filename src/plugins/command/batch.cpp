/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFileInfo>
#include <QStringList>
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "batch.h"

using namespace base;

namespace command {

PUBLISH(batch)
HELP(batch, "batch", "runs a batch of box scripts and writes plots to PNG files")

batch::batch(QString name, Box *parent)
    : Command(name, parent)
{
}

void batch::doExecute() {
    getArguments();
    getFilePaths();
    openFile();
    runFiles();
    _clipboardFile.close();
    fillClipboard();
}

void batch::getArguments() {
    switch (_args.size()) {
    case 2:
        _relativePath = _args[1];
        _fileMask = "*.box";
        break;
    case 3:
        _relativePath = _args[1];
        _fileMask = _args[2];
        break;
    default:
        ThrowException("Write: batch <path relative to input>"
                       "   or: batch <path relative to input> <file mask>");
    }
}

void batch::getFilePaths() {
    QDir dir = environment().resolveDir(Environment::Input);
    bool relPathOk = dir.cd(_relativePath);
    if (!relPathOk)
        ThrowException(dir.absolutePath() + " contains no " + _relativePath + " subfolder");
    dir.setNameFilters(QStringList() << _fileMask);
    _filePaths.clear();
    for (QFileInfo fileInfo : dir.entryInfoList(QDir::Files)) {
        _filePaths << fileInfo.absoluteFilePath();
    }
    dialog().information("Running this batch of scripts:\n\n" + _filePaths.join("\n") + "\n");
}

void batch::openFile() {
    environment().latestLoadArg("batch");
    QString filePath = environment().outputFilePath(".R");
    filePath.replace("\\", "/");
    _clipboardFile.setFileName(filePath);
    if ( !_clipboardFile.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(filePath).context(this);
    _clipboardStream.setDevice(&_clipboardFile);
}

void batch::runFiles() {
    environment().latestLoadArg("batch");
    environment().incrementFileCounter();

    _clipboardStream << "all_figures = NULL\n\n";

    for (QString filePath : _filePaths) {
        QApplication::clipboard()->clear(QClipboard::Clipboard);
        QStringList com;
        com << "run" << filePath;
        dialog().information("batch " + com.join(" "));
        _clipboardStream
            << "print('batch " << com.join(" ") << "')\n"
            << "figures = function(x) NULL\n";
        Command::submit(com);
        streamClipboard(filePath);
    }
}

void batch::streamClipboard(QString filePath) {
    QString clip = QApplication::clipboard()->text();
    clip = clip.replace("keepVariables = FALSE",
                        "keepVariables = TRUE");
    QString fileName = QFileInfo(filePath).baseName();
    _clipboardStream
            << clip
            << "all_figures = c(all_figures, list(Figure=list(FilePath='"
            << fileName
            << "', Pages=list(figures(sim)))))\n\n";
}

void batch::fillClipboard() {
    QString s =
        "source('" + _clipboardFile.fileName() + "')\n" +
        "graphics.off()\n" +
        "source('" + environment().inputFileNamePath("scripts/end-batch.R")+ "')\n" +
        "graphics.off()\n";
    environment().copyToClipboard(s);
}

}

