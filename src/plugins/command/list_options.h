/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef LIST_OPTIONS_H
#define LIST_OPTIONS_H
#include <QSet>
#include <base/convert.h>

namespace command {

enum class ListOption{
    Ports,
    Inputs,
    Outputs,
    Auxiliary,
    Imports,
    Exports,
    Boxes,
    Recurse,
    Exclusively,
    Verbose
};

typedef QSet<ListOption> ListOptionSet;

uint qHash(const ListOption &a);

}

namespace base {
    template<> command::ListOption convert(char c);
    template<> command::ListOptionSet convert(QString s);
}

#endif

