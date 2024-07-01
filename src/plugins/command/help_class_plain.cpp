/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/box.h>
#include <base/dialog.h>
#include <base/port.h>
#include "help_class_plain.h"

using namespace base;

namespace command {

HelpClassPlain::HelpClassPlain(Box *box)
    : HelpClassBase(box)
{
}

QString HelpClassPlain::header() const {
    return "\n" + _box->className() + " " + _box->help() + "\n\nInputs:\n";
}

QStringList HelpClassPlain::portLines(PortType type) const {
    QStringList list;
    for (const Port *port : _box->portsInOrder()) {
        if (port->type() == type) {
            QString item;
            item = ".";
            item +=       port->objectName().leftJustified(_colWidthName) +
                    "|" + port->value().typeName().leftJustified(_colWidthType) + "|";

            QString value = isConstant(port) ? port->value().asString() : "-";
            item += value.rightJustified(_colWidthValue);

            if (hasIntervalAsUnit(port))
                item += " " + QString().leftJustified(_colWidthUnit);
            else
                item += " " + port->unit().leftJustified(_colWidthUnit);

            item += "|";

            QString portHelp = port->help();
            if (isConstant(port)) {
                item += portHelp;
                if (hasIntervalAsUnit(port))
                    item += " " + port->unit();
            } else {
                item += " <-- " + port->unparsedExpression();
                if (!portHelp.isEmpty())
                    item += " (" + portHelp + ")";
            }
            list << item;
        }
    }
    if (list.isEmpty())
        list << "none";
    return list;
}


QString HelpClassPlain::inputPorts() const {
    return portLines(PortType::Input).join("\n") + "\n";
}

QString HelpClassPlain::outputPorts() const {
    return "\nOutputs:\n" +
           portLines(PortType::Output).join("\n") + "\n";
}

QString HelpClassPlain::additionalOutputPorts() const {
    QString additional = _box->additionalOutputs();
    return (additional.isEmpty()) ? "" : ("\n\nAdditional outputs:\n" + additional + "\n");
}

QString HelpClassPlain::footer() const {
    return "";
}

QString HelpClassPlain::sideEffects() const {
    QString side = _box->sideEffects();
    return (side.isEmpty()) ? "" : ("\n\nSide effects:\n" + side + "\n");
}

void HelpClassPlain::done(QString info) {
    dialog().information(info);
}

}
