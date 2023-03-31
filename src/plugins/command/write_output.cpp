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

WriteOutput::WriteOutput(Box *root)
    : WriteOutput(root, WriteOptionSet() << WriteOption::Source)
{
}

WriteOutput::WriteOutput(Box *root, WriteOptionSet options)
    : _root(root), _options(options)
{
    _writeInputs  = _options.contains(WriteOption::Ports) ||
                    _options.contains(WriteOption::Inputs);

    _writeOutputs = _options.contains(WriteOption::Ports) ||
                    _options.contains(WriteOption::Outputs);

    _writeAux     = _options.contains(WriteOption::Ports)||
                    _options.contains(WriteOption::Auxiliary);
    _writeSource  = _options.contains(WriteOption::Source);
    _writeFlags   = _options.contains(WriteOption::Flags);
 }

QString WriteOutput::toString() {
    toString(_root, 0);
    return _s;
}

void WriteOutput::toString(Box *box, int level) {
    // Write box
    _s += QString().fill(' ', 2*level) +
          box->className() + " " +
          box->objectName() + "{\n";

    // Write ports
    for (Port *port : box->portsInOrder()) {
        if ((_writeInputs  && port->type() == PortType::Input) ||
            (_writeOutputs && port->type() == PortType::Output) ||
            (_writeAux     && port->type() == PortType::Auxiliary) ||
            (_writeSource  && isSource(port)))
        {
            toString(port, level+1);
        }
    }

    // Write child boxes
    for (Box *child : box->findMany<Box*>("./*"))
        toString(child, level+1);

    // End box
    _s += QString().fill(' ', 2*level) + "}\n";
}

bool WriteOutput::isSource(Port *port) {
    return port->status() == PortStatus::Redefined;
}

void WriteOutput::toString(Port *port, int level) {

    QString fill = QString().fill(' ', 2*level);

    _s += fill + prefixString(port) + port->objectName() +
          " = " + assignmentString(port);
    if (_writeFlags)
        _s += " " + flags(port);
    _s += "\n";
}

QString WriteOutput::prefixString(Port *port) {
    QMap<PortType, QString> prefix = {
        {PortType::Input    , "."},
        {PortType::Output   , ">"},
        {PortType::Auxiliary, "&"}
    };
    return prefix.value(port->type());
}

QString WriteOutput::assignmentString(Port *port) {
    return
        port->unparsedExpression().isEmpty() ?
        port->value().asString() :
        port->unparsedExpression();
}

QString WriteOutput::flags(Port *port) {
    QString s = "// " + convert<QString>(port->status());
    if (port->isValueOverridden())
        s += " overridden";
    if (port->isConstant())
        s += " const";
    if (!s.isEmpty())
        s = "// " + s;
    return s;
}

}
