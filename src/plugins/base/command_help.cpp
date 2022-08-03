/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <algorithm>
#include "command_help.h"

namespace base {

QVector<CommandHelp> CommandHelp::_all;

CommandHelp::CommandHelp(QString command, QString description)
    : _command(command), _description(description)
{
    _all << *this;
}

QString CommandHelp::command() const {
    return _command;
}

QString CommandHelp::description() const {
    return _description;
}

QVector<CommandHelp> CommandHelp::all() {
    std::sort(_all.begin(), _all.end(),
              [](CommandHelp a, CommandHelp b) { return a.command() < b.command(); });
    return _all;
}

int CommandHelp::maxWidth() {
    int width(0);
    for (CommandHelp help : _all) {
        int w = help.command().size();
        if (w > width) width = w;
    }
    return width;
}

}
