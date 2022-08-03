/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BASE_PORT_TYPE_H
#define BASE_PORT_TYPE_H
#include <QString>

namespace base {

enum class PortType{Input, Output, Auxiliary};

bool isPortType(QString s);
PortType portTypeFromString(QString s);
QString portTypeToString(PortType type);

}


#endif
