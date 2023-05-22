/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command_help.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/publish.h>
#include "diagnose.h"

using namespace base;

namespace command {

PUBLISH(diagnose)
HELP(diagnose, "diagnose", "diagnoses any port value (double) outside range")

diagnose::diagnose(QString name, Box *parent)
    : Command(name, parent)
{
}

void diagnose::doExecute() {
    bool noArguments  = (_args.size() == 1),
         hasArguments = (_args.size() == 3);
    if (!noArguments && !hasArguments)
        ThrowException("Command 'debug' optionally takes two arguments");

    double minValue = -1e6, maxValue = 1e6;
    if (hasArguments) {
        bool ok1, ok2;
        minValue = QString(_args[1]).toDouble(&ok1),
        maxValue = QString(_args[2]).toDouble(&ok2);
        if (!ok1 || !ok2)
            ThrowException("Provide min and max value as arguments");
    }

    Box::diagnose(minValue, maxValue);
    Command::submit(QStringList() << "run");
    Box::diagnoseOff();
}

}
