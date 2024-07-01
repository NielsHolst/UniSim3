/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "output_text.h"

using namespace base;

namespace boxes {

PUBLISH(OutputText)

OutputText::OutputText(QString name, Box *parent)
    : Box(name, parent)
{
    help("defines output written to text file");
    Input(ports).help("Path to port(s) to output");
}

void OutputText::amend() {
    // If it does not exist, create an OutputWriter on the root
    if (!findMaybeOne<Box*>("OutputWriter::*")) {
        BoxBuilder(Box::root()).
        box("OutputWriter").name("outputWriter").endbox();
    }
}

}
