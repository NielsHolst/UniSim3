/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QMap>
#include "port_type.h"

namespace base {

static QMap<QString, PortType> _portTypeFromString =
{
    {"aux",    PortType::Auxiliary},
    {"input",  PortType::Input},
    {"output", PortType::Output}
};

static QMap<PortType, QString> _portTypeToString =
{
    {PortType::Auxiliary, "aux"   },
    {PortType::Input ,    "input" },
    {PortType::Output,    "output"}
};

bool isPortType(QString s) {
    return _portTypeFromString.contains(s);
}

PortType portTypeFromString(QString s) {
    return _portTypeFromString.value(s);
}

QString portTypeToString(PortType type) {
    return _portTypeToString.value(type);
}


}


