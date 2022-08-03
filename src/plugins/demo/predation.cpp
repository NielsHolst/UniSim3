#include <base/dialog.h>
#include <base/publish.h>
#include <base/unique_name.h>
#include <base/vector_op.h>
#include "predation.h"

using namespace base;

namespace demo {

PUBLISH(Predation)

Predation::Predation(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(apparancyFile).help("Name of file with predator-prey apparancy matrix");
}

void Predation::amend() {
    apparancy.read(apparancyFile, Table::BothLabelled);
    QStringList preyPaths = apparancy.rowNames(),
                predPaths = apparancy.colNames();
    preyPorts = findPorts(preyPaths);
    predPorts = findPorts(predPaths);

    nPrey = preyPorts.size();
    nPred = predPorts.size();
    preyDensity.resize(nPrey);
    predDensity.resize(nPred);

    setupInputPorts();
}

QVector<Port*> Predation::findPorts(QStringList paths) {
    QVector<Port*> ports;
    for (QString path : paths.toVector()) {
        ports << findOne<Port*>(path);
    }
    return ports;
}

void Predation::setupInputPorts() {
    setupInputPorts("prey_", preyPorts, preyDensity);
    setupInputPorts("pred_", predPorts, predDensity);

    QStringList names = UniqueName(predPorts).resolve();
    int i(0);
    for (Port *port : predPorts) {
        Box *parent = port->boxParent();
        Box *boxWithDemand = parent->findOne<Box*>("ancestor::*[demand]");
        QString pathToDemand = boxWithDemand->fullName() + "[demand]";
        QString portName = "demand_" + names[i];
        NamedInput(portName, predDemands[i]).imports(pathToDemand);
        ++i;
    }
}

void Predation::setupInputPorts(QString prefix,
                                const QVector<Port*> &ports,
                                QVector<double> &density) {
    QStringList names = UniqueName(ports).resolve();
    int i(0);
    for (Port *port : ports) {
        QString portName = prefix + names[i];
        NamedInput(portName, density[i]).imports(port->fullName());
        ++i;
    }
}


}
