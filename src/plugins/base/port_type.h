/* Copyright 2005-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BASE_PORT_TYPE_H
#define BASE_PORT_TYPE_H

#include <QString>

#include "base_dllexport.h"

namespace base {

enum class UNI_API PortType{Input, Output, Auxiliary};

UNI_API bool isPortType(QString s);
UNI_API PortType portTypeFromString(QString s);
UNI_API QString portTypeToString(PortType type);

}


#endif
