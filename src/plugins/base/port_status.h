/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PORT_STATUS_H
#define PORT_STATUS_H
#include "convert.h"

namespace base {

enum class PortStatus {
    Unknown,
    TypeDefault,
    ConstructionDefault,
    UserDefined
};

template<> QString convert(PortStatus status);

}

#endif

