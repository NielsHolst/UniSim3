/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/port.h>
#include "help_class_base.h"

using namespace base;

namespace command {

HelpClassBase::HelpClassBase(Box *box)
    : _box(box)
{
    Q_ASSERT(box);
}

void HelpClassBase::showClassInfo() {
    setColWidths();
    QString info =
        header() +
        inputPorts() +
        outputPorts() +
        additionalOutputPorts() +
        footer() +
        sideEffects();
    done(info);
}

void HelpClassBase::setColWidths() {
//    _colWidthName = box->additionalOutputs().isEmpty() ? QString("Outputs").size() : QString("additional").size();
    _colWidthName = QString("Outputs").size();
    _colWidthType = QString("Type").size();
    _colWidthValue = QString("Default").size();
    _colWidthUnit = 0;
    for (const Port *port : _box->findMany<Port*>(".[*]")) {
        _colWidthName = std::max(_colWidthName, port->objectName().size());
        _colWidthType = std::max(_colWidthType, port->value().typeName().size());
        if (isConstant(port))
           _colWidthValue = std::max(_colWidthValue, port->value().asString().size());
        if (!hasIntervalAsUnit(port))
           _colWidthUnit = std::max(_colWidthUnit, port->unit().size()); // + 1 for space between value and unit ??
    }
}

bool HelpClassBase::isConstant(const Port *port) const {
    return (port->expression().isEmpty() || port->expression().isConstant());
}

bool HelpClassBase::hasIntervalAsUnit(const Port *port) const {
    return port->unit().startsWith("[") || port->unit().startsWith("<") || port->unit().startsWith(">");
}

QString HelpClassBase::pluginName() const {
    Q_ASSERT(_box);
    return _box->pedigree().last().namespaceName;
}

QString HelpClassBase::className() const {
    Q_ASSERT(_box);
    return _box->pedigree().last().className;
}

}
