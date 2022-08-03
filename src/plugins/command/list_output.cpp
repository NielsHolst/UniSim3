/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command.h>
#include <base/computation.h>
#include <base/dialog.h>
#include <base/port.h>
#include <base/port_type.h>
#include "list_output.h"

using namespace base;

namespace command {

ListOutput::ListOutput(QVector<Box*> boxes, ListOptionSet options)
    : _boxes(boxes), _options(options)
{
    _listInputs  = _options.contains(ListOption::Ports) ||
                   _options.contains(ListOption::Inputs);

    _listOutputs = _options.contains(ListOption::Ports) ||
                   _options.contains(ListOption::Outputs);

    _listAux     = _options.contains(ListOption::Ports)||
                   _options.contains(ListOption::Auxiliary);

    _listImports = _options.contains(ListOption::Imports);
    _listExports = _options.contains(ListOption::Exports);
    _listShort   = _options.contains(ListOption::Short);
    _recurse     = _options.contains(ListOption::Recurse);

    if (_listImports ||_listExports) {
        if (hasRun())
            Box::root()->registerPorts();
        else
            dialog().error("Warning: You need to run the simulation before listing imports or exports");
    }
 }

bool ListOutput::hasRun() {
    if (Box::hasRoot()) {
        auto hasRun = Box::root()->findMaybeOne<Port*>("/.[hasRun]");
        return hasRun && hasRun->value<bool>();
    }
    return false;
}

QString ListOutput::toString() {
    for (Box *box : _boxes)
        toString(box, 0);
    return _s;
}

void ListOutput::toString(base::Box *box, int level) {
    // Always list box
    _s += QString().fill(' ', 2*level) +
          box->className() + " " +
          box->objectName() + "\n";

    // Sort ports
    QVector<Port*> inputs, outputs, aux;
    for (Port *port : box->portsInOrder()) {
        switch (port->type()) {
        case PortType::Input:
            inputs << port;
            break;
        case PortType::Output:
            outputs << port;
            break;
        case PortType::Auxiliary:
            aux << port;
        }
    }

    // List ports
    if (_listInputs)
        for (Port *port : inputs) toString(port, level+1);
    else {
        if (_listImports) listImports(inputs, level);
        if (_listExports) listExports(inputs, level);
    }

    if (_listOutputs)
        for (Port *port : outputs) toString(port, level+1);
    else {
        if (_listImports) listImports(outputs, level);
        if (_listExports) listExports(outputs, level);
    }

    if (_listAux)
        for (Port *port : aux) toString(port, level+1);
    else {
        if (_listImports) listImports(aux, level);
        if (_listExports) listExports(aux, level);
    }

    // List children
    if (_recurse)
        for (Box *child : box->findMany<Box*>("./*"))
            toString(child, level+1);
}

void ListOutput::listImports(QVector<base::Port*> ports, int level) {
    for (Port *port : ports) {
        if (port->importPorts().size() > 0)
            toString(port, level+1);
    }
}

void ListOutput::listExports(QVector<base::Port*> ports, int level) {
    for (Port *port : ports) {
        if (port->exportPorts().size() > 0)
            toString(port, level+1);
    }
}

void ListOutput::toString(base::Port *port, int level) {

    QString fill = QString().fill(' ', 2*level);

    _s += fill + prefixString(port) + port->objectName() +
          " = " + assignmentString(port) + "\n";

    if (_listImports) {
        for (Port *import : port->importPorts())
            _s += fill + "  << " + import->fullName() + "\n";
    }

    if (_listExports) {
        for (Port *export_ : port->exportPorts())
            _s += fill + "  >> " + export_->fullName() + "\n";
    }
}

QString ListOutput::prefixString(base::Port *port) {
    QMap<PortType, QString> prefix = {
        {PortType::Input    , "."},
        {PortType::Output   , ">"},
        {PortType::Auxiliary, "&"}
    };
    return _listShort ? "" : prefix.value(port->type());
}

QString ListOutput::assignmentString(base::Port *port) {
    QString s = port->value().asString();
    if (!_listShort) {
        s += port->unit().isEmpty() ? "" : (" " + port->unit());
        s += port->isConstant() ? " const" : "";
        s += port->unparsedExpression().isEmpty() ? "" : (" <- " + port->unparsedExpression());
    }
    return s;
}

}
