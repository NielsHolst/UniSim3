/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/dialog.h>
#include <base/exception.h>
#include <base/port.h>
#include <base/publish.h>
#include "modifier.h"

using namespace base;

namespace boxes {

PUBLISH(Modifier)

Modifier::Modifier(QString name, Box *parent)
    : Box(name, parent)
{
    help("modifies ports of existing boxes");
    Input(boxes).help("Vector of paths to boxes");
    Input(report).equals(true).help("Report modified ports?");
}

void Modifier::initialize() {
    findBoxPaths();
    findPortNames();
    for (QString &portName : _portNames) {
        // Find the destination ports for this port name
        QVector<base::Port*> destPorts = Modifier::findDestinationPorts(portName);
        // Find the source port
        Port *sourcePort = findOne<Port*>(".[" + portName + "]");
        if (sourcePort->hasImport())
            for(Port *destPort : destPorts) destPort->imports(sourcePort->importPath());
        else
            for(Port *destPort : destPorts) destPort->equals(sourcePort->valueAsString());
    }
}

void Modifier::findBoxPaths() {
    _boxPaths.clear();
    QStringList paths = QStringList(Modifier::boxes.begin(), Modifier::boxes.end());
    for (Box *box : findMany<Box*>(paths.join("|"))) {
        _boxPaths << box->fullName();
//        dialog().information(box->fullName());
    }
}

void Modifier::findPortNames() {
    _portNames.clear();
    for (Port *port : findMany<Port*>(".[*]")) {
        if (port->isBlind()) {
            _portNames << port->name();
//            dialog().information(port->name());
        }
    }
}

QVector<base::Port*> Modifier::findDestinationPorts(QString portName) {
//    dialog().information("findDestinationPorts " + portName);
    QVector<base::Port*> ports;
    for (QString &path : _boxPaths) {
        QString destPath = path + "[" + portName + "]";
        Port *port = findMaybeOne<Port*>(destPath);
        if (port) {
            ports << port;
//            dialog().information(port->fullName());
        }
    }
    if (ports.isEmpty())
        ThrowException("Found no ports of this name in these boxes").
                value(portName).
                value2(_boxPaths.join(", ")).
                context(this);
    return ports;
}

} //namespace
