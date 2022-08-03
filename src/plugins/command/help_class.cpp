/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <stdlib.h>
#include <base/box.h>
#include <base/command_help.h>
#include <base/convert.h>
#include <base/dialog.h>
#include <base/exception.h>
// #include <base/factory_plug_in.h>
#include <base/mega_factory.h>
#include <base/publish.h>
#include "help_class.h"

using namespace base;

namespace command {

PUBLISH(help_class)
HELP(help_help_class, "help <name>", "shows command or class documentation")

help_class::help_class(QString name, QObject *parent)
    : Command(name, parent), _box(0)
{
}

void help_class::doExecute() {
    bool hasOption = false;
    if (_args.size() >= 2 && _args.size() <= 3) {
        if (_args.size() == 3) {
            QString option = _args.at(2);
            if (option.toLower() == "x")
                hasOption = true;
            else
                dialog().error("Wrong option. Only 'x' allowed");
        }
        QString className = _args.at(1);
        if (getPlugIn(className)) {
            _expand = hasOption;
            writePlugInHelp();
        }
        else if (createBox(className)) {
            _expand = true;
            writeClassHelp();
            _box->deleteLater();
        }
        else
            dialog().error("Class name '" + className + " 'not found");
    }
    else
        dialog().error("Write: 'help <class or plug-in name> [x]' ");
}

bool help_class::getPlugIn(QString name) {
    _plugIn = nullptr;
    for (auto factory : MegaFactory::factories()) {
        if (name == factory->id()) {
            _plugIn = factory;
        }
    }
    return _plugIn;
}

bool help_class::createBox(QString className) {
    try {
        Box::saveCurrentRoot();
        _box = MegaFactory::create<Box>(className, "helpObject", 0);
        Box::restoreCurrentRoot();
    }
    catch (Exception &) {
        _box = nullptr;
    }
    return _box;
}

void help_class::writePlugInHelp() {
    for (QString className : _plugIn->inventory()) {
        createBox(className);
        writeClassHelp();
        _box->deleteLater();
    }
}

namespace {

    bool showValue(const Port *port) {
        return port->access() == PortAccess::Input && !port->hasImport();
    }

//    QString value(const Port *port) {
//        return port->hasImport() ? port->importPath() : port->valueAsString();
//    }
}

void help_class::setColWidths() {
    _colWidthName = _colWidthValue = _colWidthUnit = 0;
    for (const Port *port : _box->findMany<Port*>(".[*]")) {
        _colWidthName = std::max(_colWidthName, port->objectName().size());
        if (showValue(port))
            _colWidthValue = std::max(_colWidthValue, port->valueAsString().size());
        _colWidthUnit = std::max(_colWidthUnit, port->unit().size());
    }
}

void help_class::writeClassHelp() {
    setColWidths();
    QString msg = _expand ? "\n" : "";
    msg += _box->className() + " " + _box->help();
    if (_expand) {
        msg += "\n\nInput:\n" +
               portsHelp(PortAccess::Input).join("\n") +
               "\n\nOutput:\n" +
               portsHelp(PortAccess::Output).join("\n");
        if (!_box->sideEffects().isEmpty())
            msg += "\n\nSide effects:\n" + sideEffects();
    }
    dialog().information(msg);
}

QStringList help_class::portsHelp(PortAccess access) {
    QStringList list;
    for (const Port *port : _box->findMany<Port*>(".[*]")) {
        if (port->access() == access) {
            QString item;
            item = "." + port->objectName().leftJustified(_colWidthName);
            QString value =  showValue(port) ? port->valueAsString() : "";
            item += "|" + value.rightJustified(_colWidthValue);
            if (_colWidthUnit > 0)
                item += " " + port->unit().leftJustified(_colWidthUnit);
            item += "|" + port->help();
            list << item;
        }
    }
    if (list.isEmpty())
        list << "none";
    return list;
}

QString help_class::sideEffects() {
    QStringList lines = _box->sideEffects().split("\n");
    QString s;
    for (int i = 0; i < lines.size(); ++i)
        s += QString("(%1) %2\n").arg(i+1).arg(lines.at(i));
    return s;
}

}
