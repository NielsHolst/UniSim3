/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
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
    : Command(name, parent), _isFirstFigure(true)
{
}

void batch::doExecute() {
    getArguments();
    if (_fileMask.isEmpty()) {
        _boxFilePaths = getFilePaths("*.box");
        _batchRFilePaths = getFilePaths("batch.txt");
    }
    else {
        _userFilePaths = getFilePaths(_fileMask);
    }
    openFile();
    runBoxFiles(_userFilePaths.isEmpty() ? _boxFilePaths : _userFilePaths);
    sourceRFiles();
    closeFile();
    fillClipboard();
}

void batch::getArguments() {
    switch (_args.size()) {
    case 2:
        _relativePath = _args[1];
        _fileMask = "";
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

QStringList batch::getFilePaths(QString fileMask) {
    QDir dir = environment().resolveDir(Environment::Input);
    if (!dir.cd(_relativePath))
        ThrowException(dir.absolutePath() + " contains no " + _relativePath + " subfolder");
    return getFilePaths(dir, fileMask);
}

QStringList batch::getFilePaths(QDir dir, QString fileMask) {
    static QRegularExpression re(R"([^(]*\([0-9]*\).*)");
    QStringList filePaths;
    dir.setNameFilters(QStringList() << fileMask);
    for (const QFileInfo &fileInfo : dir.entryInfoList(QDir::Files)) {
        // Discard file names with a version number (parenthesized number) in them
        QString s = fileInfo.absoluteFilePath();
        if (!re.match(s).hasMatch())
            filePaths << s;
    }
    for (const QFileInfo &fileInfo : dir.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot)) {
        filePaths << getFilePaths(fileInfo.absoluteFilePath(), fileMask);
    }
    return filePaths;
}

void batch::openFile() {
    environment().latestLoadArg("batch");
    QString filePath = environment().outputFilePath(".R");
    filePath.replace("\\", "/");
    _file.setFileName(filePath);
    if ( !_file.open(QIODevice::WriteOnly | QIODevice::Text) )
        ThrowException("Cannot open file for output").value(filePath).context(this);
    _fileStream.setDevice(&_file);
}

void batch::closeFile() {
    _file.close();
}

void batch::runBoxFiles(QStringList boxFilePaths) {
    dialog().information("Running scripts:\n\n" + boxFilePaths.join("\n") + "\n");

    environment().latestLoadArg("batch");
    environment().incrementFileCounter();

    _fileStream << "all_figures = NULL\n\n";

    QStringList errors;
    for (const QString &filePath : boxFilePaths) {
        QApplication::clipboard()->clear(QClipboard::Clipboard);
        QStringList com;
        com << "run" << filePath;
        dialog().information("\nbatch " + com.join(" "));
        _fileStream
            << "print('batch " << com.join(" ") << "')\n"
            << "figures = function(x) NULL\n";
        Command::submit(com);
        Command::submit(QStringList() << "clip");
        if (dialog().errorCount() > 0)
            errors << filePath;
        writeClipboard();
        writeFigure(filePath, ".box", pagesExpected(filePath));
    }
    if (!errors.isEmpty())
        dialog().error("");
    for (const QString &error : errors)
        dialog().error("Runtime error in " + error);
    if (!errors.isEmpty())
        dialog().error("");
}

void batch::sourceRFiles() {
    for (const QString &batchRFilePath : _batchRFilePaths) {
        QString s = readFile(batchRFilePath);
        QStringList RFileNames = s.split("\n",Qt::SkipEmptyParts);
        for (const QString &RFileName : RFileNames) {
            auto fi = QFileInfo(batchRFilePath);
            QString filePath = fi.absolutePath() + "/" + RFileName.trimmed();
            _fileStream
                << "print('source " << filePath << "')\n"
                << "figures = function(x) NULL\n"
                << "source(\"" << filePath << "\")\n";
            writeFigure(filePath, ".R", fileContainsGgplot(filePath));
        }

    }
}

QString batch::readFile(QString filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        ThrowException("Cannot open file for input").value(filePath);
    return file.readAll();
}

bool batch::fileContainsGgplot(QString filePath) const {
    QString s = readFile(filePath);
    return (s.indexOf("ggplot") > -1);
}

bool batch::pagesExpected(QString filePath) {
    static QRegularExpression re(R"("[^"]*")");
    bool expected = false;
    QString s = readFile(filePath), s2;
    if (s.indexOf("PageR") > -1) {
        expected = true;
    }
    else {
        auto i = s.indexOf(".scripts");
        if (i > -1) {
            s2 = s.mid(i);
            QRegularExpressionMatch match = re.match(s2);
            QStringList texts = match.capturedTexts();
            if (!texts.isEmpty()) {
                QString fn = texts.at(0);
                auto fi = QFileInfo(filePath);
                expected = fileContainsGgplot(fi.absolutePath() + "/" + fn.mid(1, fn.size()-2));
            }
        }
    }
    return expected;
}

void batch::writeClipboard() {
    QString clip = QApplication::clipboard()->text();
    clip = clip.replace("keepVariables = FALSE",
                        "keepVariables = TRUE");
    _fileStream << clip;
}

void batch::writeFigure(QString filePath, QString fileType, bool pagesExpected) {
    auto from = filePath.indexOf("/input/") + 7;
    auto fi = QFileInfo(filePath.mid(from));
    QString path = fi.path(),
            baseName = fi.baseName();

    _fileStream
            << "all_figures = ";
    if (!_isFirstFigure) {
        _fileStream
            << "c(all_figures, ";
    }
    _fileStream
            << "list(Figure=list("
            << "FilePath='" << path << "', "
            << "FileBaseName='" << baseName << "', "
            << "FileType='" << fileType << "', "
            << "Pages=list(figures(sim)), PagesExpected=" << (pagesExpected ? "TRUE" : "FALSE")
            << "))";
    if (!_isFirstFigure) {
        _fileStream
            << ")";
    }
    _fileStream
            << "\n\n";
    _isFirstFigure = false;
}

void batch::fillClipboard() {
    QString s =
        "source('" + _file.fileName() + "')\n" +
        "graphics.off()\n" +
        "source('" + environment().inputFileNamePath("scripts/end-batch.R")+ "')\n" +
        "graphics.off()\n";
    environment().copyToClipboard(s);
}

}

