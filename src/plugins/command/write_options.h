/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef WRITE_OPTIONS_H
#define WRITE_OPTIONS_H
#include <QSet>
#include <base/convert.h>

namespace command {

enum class WriteOption{
    Ports,
    Inputs,
    Outputs,
    Auxiliary,
    Source,
    Flags,
    Edit
};

typedef QSet<WriteOption> WriteOptionSet;

uint qHash(const WriteOption &a);

}

namespace base {
    template<> command::WriteOption convert(char c);
    template<> command::WriteOptionSet convert(QString s);
}

#endif

