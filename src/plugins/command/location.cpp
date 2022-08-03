/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QLocale>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/exception.h>
#include <base/publish.h>
#include "location.h"

using namespace base;

namespace command {

PUBLISH(location)
HELP(location, "location", "shows international location setting")

location::location(QString name, Box *parent)
    : Command(name, parent)
{
}


void location::doExecute() {
    if (_args.size() > 1)
        ThrowException("'location' takes no arguments");
    QLocale locale;
    dialog().information(
        "Country: " + QLocale::countryToString( locale.country() ) +
        "\nDecimal character in output: \"" + QString(locale.decimalPoint()) + "\""
    );
}


}
