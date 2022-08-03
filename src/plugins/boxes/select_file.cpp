/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QDir>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "select_file.h"


using namespace base;

namespace boxes {

PUBLISH(SelectFile)

SelectFile::SelectFile(QString name, Box *parent)
    : Box(name, parent)
{
    help("selects a file from a folder");
    Input(folder).help("Relative path to folder");
    Input(filter).equals("*.txt").help("File filter");
    Input(selectFileNumber).equals(1).help("Which file to pick");
    Input(fileNumberOffset).equals(1).help("Offset for fileNumber, typically 0 or 1");
    Input(showFileNames).equals(true).help("Show set of file names?").unit("bool");
    Output(numFiles).noClear().help("Number of matching files");
    Output(fileName).help("Name of selected file");
    Output(fileNamePath).help("Name of selected file with absolute path");
}


void SelectFile::amend() {
    QDir dir = environment().currentBoxScriptFolder();
    dir.cd(folder);
    _files = dir.entryList(QStringList() << filter, QDir::Files);
    numFiles = _files.size();
    if (numFiles == 0)
        ThrowException("No files found").value(folder).value2(filter).context(this);
    reset(1);
}

void SelectFile::initialize() {
    if (showFileNames) {
        dialog().information(QString("Select among %1 files (%2) from %3")
                             .arg(_files.size()).arg(filter).arg(folder));
        for (QString fileName : _files.toVector()) {
            dialog().information(fileName);
        }
    }
}

void SelectFile::reset() {
    reset(selectFileNumber);
}

void SelectFile::reset(int fileNumber) {
    int i = fileNumber - fileNumberOffset;
    fileName = _files.at(i);
    fileNamePath = folder + "/" + fileName;
}

}
