/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/box_builder.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/publish.h>
#include "test.h"

using namespace base;

namespace command {

PUBLISH(test)
HELP(test, "test", "loads test box")

test::test(QString name, QObject *parent)
    : Command(name, parent)
{
}

void test::doExecute() {
    BoxBuilder builder;
    builder.clear();
    builder.
        box("Simulation").name("FibSim").
            port("steps").equals(4).
            box("Fibonacci").name("fibonacci").
            endbox().
        endbox();

    Environment &env(environment());
    env.root(builder.content());

    DialogBase &dia(dialog());
    dia.information("Loaded FibSim");
}

}
