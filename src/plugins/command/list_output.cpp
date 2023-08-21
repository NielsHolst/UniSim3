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
#include <base/value.h>
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
    _recurse     = _options.contains(ListOption::Recurse);
    _listVerbose = _options.contains(ListOption::Verbose);

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
          " = " + port->value().asString();
    if (!port->unit().isEmpty())
        _s += " " + port->unit();
    if (port->isConstant())
        _s += " const";
    bool oneImportedElement = (port->expression().stack().size() == 1 &&
                               Expression::type(port->expression().stack().at(0)) != Expression::Type::Value),
         manyElements = (port->expression().stack().size() > 1);
    if (oneImportedElement || manyElements)
        _s += " <- " + port->unparsedExpression();
    _s += "\n";

    if (_listVerbose)
        _s += verboseInfo(port, QString().fill(' ', 2*(level+1)));

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
    return prefix.value(port->type());
}

QString ListOutput::verboseInfo(base::Port *port, QString fill) {
    const Expression
        &expression(port->expression());
    const Expression::Stack
        &stack(   expression.stack() ),
        &original(expression.original() );
    QString s =
        fill + "order        : " + QString::number(port->order()) + "\n" +
        fill + "port type    : " + portTypeToString(port->type()) + "\n" +
        fill + "status       : " + convert<QString>(port->status()) + "\n" +
        fill + "unparsed expr: " + "\"" + port->unparsedExpression() + "\"\n" +
        fill + "value type   : " + port->value().typeName() + "\n" +
        fill + "value size   : " + QString::number(port->value().size()) + "\n" +
        fill + "expr size    : " + QString::number(expression.size()) + "\n" +
        fill + "expr const   : " + (expression.isConstant() ? "yes\n" : "no\n") +
        fill + "port exports : " + Node::fullNames(port->exportPorts()).join("  \n") + "\n" +
        fill + "port imports : " + Node::fullNames(port->importPorts()).join("  \n") + "\n" +
        fill + "port imp leav: " + Node::fullNames(port->importPortsLeaves()).join("  \n") + "\n" +
        fill + "expr imports : " + expression.importPortNames().join("  \n") + "\n" +
        fill + "orig as str  : " + expression.originalAsString() + "\n" +
        fill + "stack as str : " + expression.stackAsString() + "\n" +
        fill + "original     : " + Expression::toString(original, " ", true) + "\n" +
        fill + "stack        : " + Expression::toString(stack, " ", true) + "\n";
    return s;
}

}
