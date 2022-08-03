/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box.h>
#include <base/command_help.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/mega_factory.h>
#include <base/port.h>
#include <base/publish.h>
#include "doc.h"

using namespace base;

namespace command {

PUBLISH(doc)
HELP(doc, "doc", "writes model documentation file")

doc::doc(QString name, Box *parent)
    : Command(name, parent)
{
    }

void doc::doExecute() {
    if (_args.size() > 2)
        ThrowException("Command 'doc' takes at most one argument");
    dialog().resetErrorCount();
    doLoad();
    if (dialog().errorCount() == 0) {
        Command::submit(QStringList() << "list");
        doDoc();
    }
}

void doc::doLoad() {
    QStringList com;
    com << "load";
    if (_args.size() == 2)
        com << _args[1];
    Command::submit();
}

void doc::doDoc() {
    environment().openOutputFile(_file, "txt");
    _text.setDevice(&_file);
    appendHeadings();
    appendText(Box::root());
    _file.close();
    dialog().information("Documentation file written to '" + environment().latestOutputFilePath("txt") + "'");
    environment().incrementFileCounter();
}

void doc::appendHeadings() {
    _text << "Class\t"
          << "ObjectName\t"
          << "InputName\t"
          << "Value\t"
          << "Unit\t"
          << "ValueFrom\t"
          << "XmlPath\t"
          << "XmlName\n";
}


void doc::appendText(Box *box) {
    for (auto child : box->children()) {
        Port *port = dynamic_cast<Port*>(child);
        if (port && port->access()==PortAccess::Input) {
            QString source  = port->attributes().contains("source") ?
                              port->attribute("source") : "",
                    extName = port->attributes().contains("externalName") ?
                              port->attribute("externalName") : "";
            _text << qualifiedClassName(box->className())
                  << "\t" << box->fullName()
                  << "\t" << port->name()
                  << "\t" << port->valueAsString()
                  << "\t" << port->unit()
                  << "\t" << (port->hasImport() ? port->importPath() : QString())
                  << "\t" << source
                  << "\t" << extName
                  << "\n";
        }
    }

    for (auto child : box->children()) {
        Box *box = dynamic_cast<Box*>(child);
        if (box)
            appendText(box);
    }
}

QString doc::qualifiedClassName(QString className) {
    // There really should be only one qualified name found
    QStringList names = MegaFactory::qualifiedClassNames(className);
    return names.join("|");
}

}
