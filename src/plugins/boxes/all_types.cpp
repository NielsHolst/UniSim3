/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "all_types.h"

using namespace base;

namespace boxes {

PUBLISH(AllTypes)

AllTypes::AllTypes(QString name, Box *parent)
    : Box(name, parent)
{
    help("contains one port of each types - useful to learn about types");
    NamedInput("bool", _bool);
    NamedInput("int", _int);
    NamedInput("double", _double);
    NamedInput("string", _string);
    Input(date);
    Input(time);
    Input(datetime);
    Input(baredate);
    Input(path);
    Input(vec_bool);
    Input(vec_int);
    Input(vec_double);
    Input(vec_string);
    Input(vec_date);
    Input(vec_time);
    Input(vec_datetime);
    Input(vec_baredate);
}

} //namespace

