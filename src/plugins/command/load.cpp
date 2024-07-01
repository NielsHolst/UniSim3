/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <string>
#include <sstream>
#include <QFileInfo>
#include <base/box.h>
#include <base/box_builder.h>
#include <base/command_help.h>
#include <base/computation.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include <base/reader_boxscript.h>
#include <base/reader_xml.h>
#include "load.h"

using std::unique_ptr;
using namespace base;

namespace command {

PUBLISH(load)
HELP(load, "load", "loads a boxscript")

load::load(QString name, Box *parent)
    : Command(name, parent)
{
}

void load::doExecute() {
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
        ThrowException("Too many arguments. Write 'load <file-name>'.").
                hint("If the file name contains spaces then enclose it in apostrophes").
                value(_args.join(" "));
    }
    env.currentLoadArg(fileName);
    readFile(fileName);
    env.latestLoadArg(fileName);
}

void load::readFile(QString fileName) {
    QString fileNamePath = environment().inputFileNamePath(fileName);

    // Builder and reader collaborate to create model
    BoxBuilder builder;
    unique_ptr<ReaderBase> reader;
    if (fileName.endsWith("box"))
        reader = unique_ptr<ReaderBase>( new ReaderBoxScript(&builder) );
    else if (fileName.endsWith("xml")) {
        reader = unique_ptr<ReaderBase>( new ReaderXml(&builder) );
    }
    else {
        ThrowException("Can only open files of type .box or .xml").value(fileNamePath);
    }

    // Delete current model
    Box::root(nullptr);

    // Create a new model
    try {
        reader->parse(fileNamePath);
        Box::root(builder.root());
    }
    catch (const Exception &ex) {
        dialog().error(ex.what());
    }    

    // Report on created model
    int n = Box::root()->findMany<Box*>("*").size();
    dialog().information(QString("%1 boxes created").arg(n));

    // Ready for any action
    Computation::changeStep(Computation::Step::Ready);
}


}
