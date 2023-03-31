/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <ctype.h>
#include <base/exception.h>
#include "write_options.h"

namespace command {

uint qHash(const WriteOption &a) {
    return static_cast<int>(a);
}

}

using namespace command;

namespace base {

template<> WriteOption convert(char c) {
    WriteOption o;
    switch (c) {
    case 'p': o = WriteOption::Ports; break;
    case 'i': o = WriteOption::Inputs; break;
    case 'o': o = WriteOption::Outputs; break;
    case 'a': o = WriteOption::Auxiliary; break;
    case 's': o = WriteOption::Source; break;
    case 'f': o = WriteOption::Flags; break;
    case 'e': o = WriteOption::Edit; break;
    default: ThrowException("Unknown write option")
                .value(c).hint("Valid options: piosfe");
    }
    return o;
}

template<> WriteOptionSet convert(QString s) {
    WriteOptionSet ops;
    if (s.isEmpty())
        ops << WriteOption::Source;
    else for (int i=0; i<s.size(); ++i)
        ops << convert<command::WriteOption>(s.at(i).toLatin1());
    return ops;
}

}

