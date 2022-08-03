/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QFile>
#include "boxscript_parser.h"
#include "boxscript_preprocessor.h"
#include "computation.h"
#include "reader_boxscript.h"

using namespace std;

namespace base {

ReaderBoxScript::ReaderBoxScript(BoxBuilder *builder)
    : ReaderBase(builder)
{
}

void ReaderBoxScript::parse(QString filePath) {
    // Change the computation step
    Computation::changeStep(Computation::Step::Construct);

    // Read and pre-process boxscript
    BoxScriptPreprocessor preprocessor;
    QString source = preprocessor.preprocess(filePath);

    // Parse
    boxscript::parser::Result ast = boxscript::parser::parse(source.toStdString(), filePath.toStdString());
    ast->root.build(_builder);
}


} // namespace
