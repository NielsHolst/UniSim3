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
    if (_args.size() != 3 && _args.size() != 4)
        ThrowException("Command 'debug' takes two or three arguments");

    bool ok1, ok2;
    double minValue = QString(_args[1]).toDouble(&ok1),
           maxValue = QString(_args[2]).toDouble(&ok2);
    if (!ok1 || !ok2)
        ThrowException("Provide min and max value as arguments");

    QString path = (_args.size() == 3) ? "" : _args[3];

    Box::diagnose(minValue, maxValue, path);
    Command::submit(QStringList() << "run");
    Box::diagnoseOff();
}

}
