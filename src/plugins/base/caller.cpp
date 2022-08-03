/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "caller.h"

namespace base {

Caller::Caller()
    : _caller(nullptr), _file(""), _line(0)
{
}

Caller::Caller(const Node *caller, QString file, int line)
    : _caller(caller), _file(file), _line(line)
{
}

}

