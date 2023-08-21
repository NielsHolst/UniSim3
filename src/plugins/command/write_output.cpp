/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
//#include <base/command.h>
//#include <base/computation.h>
//#include <base/dialog.h>
#include <base/port.h>
#include <base/port_type.h>
#include "write_output.h"

using namespace base;

namespace command {

WriteOutput::WriteOutput(Box *root, Option option)
    : _root(root), _option(option)
{
}

QString WriteOutput::toString() {
    toString(_root, 0);
    return _s;
}

void WriteOutput::toString(Box *box, int level) {
    // Skip this box
    if (!box->doWriteOnCommand())
        return;

    // Write box
    _s += QString().fill(' ', 2*level) +
          box->className() + " " +
          box->objectName() + "{\n";

    // Write ports
    for (Port *port : box->portsInOrder()) {
        bool drop = false;
        if (_option == WriteOutput::Option::WriteUserScript) {
            if (port->type() == PortType::Output)
                drop = true;
            else if (port->type() == PortType::Input && port->status() != PortStatus::UserDefined)
                drop = true;
        }
        if (!drop)
            toString(port, level+1);
    }

    // Write child boxes
    for (Box *child : box->findMany<Box*>("./*"))
        toString(child, level+1);

    // End box
    _s += QString().fill(' ', 2*level) + "}\n";
}


void WriteOutput::toString(Port *port, int level) {
    QString fill = QString().fill(' ', 2*level);
    _s += fill + prefixString(port) + port->objectName() +
          " = " + assignmentString(port);
    if (_option == WriteOutput::Option::WriteAll)
        _s += " " + flags(port);
    _s += "\n";
}

QString WriteOutput::prefixString(Port *port) {
    QMap<PortType, QString> prefix = {
        {PortType::Input    , "."},
        {PortType::Output   , "//"},
        {PortType::Auxiliary, "&"}
    };
    return prefix.value(port->type());
}

QString WriteOutput::assignmentString(Port *port) {
    QString s =
        (port->isConstant() || port->type() == PortType::Output) ?
        port->value().asString() :
        port->unparsedExpression();
    return s.isNull() ? "" : s;
}

QString WriteOutput::flags(Port *port) {
    QString info;
    switch (port->type()) {
    case PortType::Input:
        info = convert<QString>(port->status());
        break;
    case PortType::Output:
        info = "output";
        break;
    case PortType::Auxiliary:
        info = "aux";
        break;
    }
    return "// " + info + " " +
           (port->isConstant() ? "constant" : "variable");
}

}
