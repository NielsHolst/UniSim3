/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iostream>
#include "box.h"
#include "computation.h"
#include "dialog.h"
#include "environment.h"
#include "exception.h"
#include "mega_factory.h"
#include "path.h"
#include "port.h"
#include "timer.h"

namespace base {

std::unique_ptr<Box> Box::_root;
Box *Box::_latest  = nullptr;
bool Box::_debugOn = false;

Box::Box(QString name, Box *parent)
    : Node(name, parent), _amended(false), _initialized(false), _cloned(false), _timer(this)
{
    help("has no documented functionality");
    setClassName("base", "Box");
    if (!_latest || parent)
        _latest = this;
}

Box::~Box() {
    if (this == _latest)
        _latest = nullptr;
}

void Box::addPort(Port *port) {
    addPort(_portMap, port);
    port->setOrder(_portsInOrder.size());
    _portsInOrder << port;
}

void Box::addPort(QMap<QString,Port*> &ports, Port *port) {
    QString name{port->objectName()};
    if (ports.contains(name))
        ThrowException("Box already has a port with this name").value(name).context(this);
    ports[name] = port;

}

Port* Box::peakPort(QString name) {
    return _portMap.contains(name) ? _portMap.value(name) : nullptr;
}

const Port* Box::peakPort(QString name) const {
    return _portMap.contains(name) ? _portMap.value(name) : nullptr;
}

Port* Box::port(QString name, Caller caller) {
    Port *port = peakPort(name);
    if (!port)
        ThrowException("No port of that name in this box").value(name).context(this).caller(caller);
    return port;
}

const Port* Box::port(QString name, Caller caller) const {
    const Port *port = peakPort(name);
    if (!port) {
        ThrowException("No port of that name in this box").value(name).context(this).caller(caller);
    }
    return port;
}

void Box::help(QString s) {
    _help = s;
}

QString Box::help() const {
    // Use help text only in derived classes
    return (className()=="Box") ? "is just as a container box" : _help;
}

void Box::sideEffects(QString s) {
    _sideEffects = s;
}

QString Box::sideEffects() const {
    return _sideEffects;
}

void Box::additionalOutputs(QString description) {
    _additionalOutputs = description;
}

QString Box::additionalOutputs() const {
    return _additionalOutputs;
}

bool Box::hasRoot() {
    return _root.get();
}

Box* Box::root(Box *box) {
    if (!box)
        _root.reset();
    else {
        auto newRoot = std::unique_ptr<Box>(box);
        _root = std::move(newRoot);
        environment().current(_root.get());
    }
    return _root.get();
}

Box* Box::root() {
    if (Computation::currentStep() <= Computation::Step::Amend ||
        Computation::currentStep() == Computation::Step::Scratch)
        return _latest ? _latest->latestRoot() : nullptr;
    else if (!_root)
        ThrowException("Missing root box");
    return _root.get();
}

Box *Box::latestRoot() {
    Box *p = parent<Box*>();
    return p ? p->latestRoot() : this;
}

QString Box::profileReport() {
    QString result;
    auto report = collectTimerReport();
    double total = 0.;
    for (auto rep : report)
        total += rep.duration;
    std::sort(report.begin(), report.end(),
              [](Timer::ReportItem a, Timer::ReportItem b) { return a.duration > b.duration; });
    for (auto item : report) {
        result +=
                item.parent->fullName() + "\t" +
                item.name + "\t" +
                QString::number(item.duration) + "\t" +
                QString::number(round(100*item.duration/total)) + "\n";
    }
    return result;
}

Timer::Report Box::collectTimerReport() {
    Timer::Report result = _timer.report();
    for (auto box : children<Box*>())
        result.append(box->collectTimerReport());
    return result;
}

void Box::run() {
    ThrowException("Method 'run' not defined for this class").value(className()).context(this);
}

void Box::amendFamily() {
    if (!_amended)
        createTimers();
    _timer.start("amend");
    for (auto box : children<Box*>())
        box->amendFamily();
    if (!_amended) {
        if (_debugOn)
            dialog().information("Amending " + fullName());
        setCurrent(this);
        amend();
    }
    touchPorts();
    _timer.stop("amend");
    _amended = true;
}

void Box::createTimers() {
    createTimer("amend");
    createTimer("initialize");
    createTimer("reset");
    createTimer("update");
    createTimer("cleanup");
    createTimer("debrief");
    createTimer("evaluatePorts");
}

void Box::createTimer(QString name) {
    _timer.addProfile(name);
}

void Box::startTimer(QString name) {
    _timer.start(name);
}

void Box::stopTimer(QString name) {
    _timer.stop(name);
}

void Box::initializeFamily() {
    if (!_amended)
        ThrowException("Box must be amended before initialisation").context(this);
    if (_initialized)
        ThrowException("Box already initialized").context(this);
    _initialized = true;

    if (this==root())
        Node::enumerate();

    _timer.reset();
    for (auto box : children<Box*>())
        box->initializeFamily();
    evaluatePorts();
    _timer.start("initialize");
    if (_debugOn)
        dialog().information("Initializing " + fullName());
    setCurrent(this);
    initialize();
    _timer.stop("initialize");
}

void Box::resetFamily() {
    for (auto box : children<Box*>())
        box->resetFamily();
    clearPorts();
    evaluatePorts();
    _timer.start("reset");
    if (_debugOn)
        dialog().information("Resetting " + fullName());
    setCurrent(this);
    reset();
    verifyPorts();
    _timer.stop("reset");
}

void Box::updateFamily() {
    for (auto box : children<Box*>())
        box->updateFamily();
    evaluatePorts();
    _timer.start("update");
    if (_debugOn)
        dialog().information("Updating " + fullName());
    setCurrent(this);
    update();
    verifyPorts();
    _timer.stop("update");
}

void Box::cleanupFamily() {
    for (auto box : children<Box*>())
        box->cleanupFamily();
    evaluatePorts();
    _timer.start("cleanup");
    if (_debugOn)
        dialog().information("Cleaning up " + fullName());
    setCurrent(this);
    cleanup();
    verifyPorts();
    _timer.stop("cleanup");
}

void Box::debriefFamily() {
    for (auto box : children<Box*>())
        box->debriefFamily();
    evaluatePorts();
    _timer.start("debrief");
    if (_debugOn)
        dialog().information("Debriefing " + fullName());
    setCurrent(this);
    debrief();
    verifyPorts();
    _timer.stop("debrief");
}

bool Box::isInitialized() const {
    return _initialized;
}

const QVector<Port*> &Box::portsInOrder() {
    return _portsInOrder;
}

void Box::clearPorts() {
    for (Port *port : _portsInOrder)
        port->clear();
}

void Box::touchPorts() {
    for (Port *port : _portsInOrder)
        port->touch();
}

void Box::evaluatePorts() {
    _timer.start("evaluatePorts");
    for (Port *port : _portsInOrder)
        port->evaluate();
    _timer.stop("evaluatePorts");
}

void Box::verifyPorts() {
    if (_debugOn) {
        for (Port *port : _portsInOrder)
            port->verifyValue();
    }
}

void Box::registerPorts() {
    for (Box *box : children<Box*>())
        box->registerPorts();
    for (Port *port : _portsInOrder)
        port->registerExports();
}

Box* Box::clone(QString name, Box *parent) {
    // Clone me
    Box *myClone = MegaFactory::create<Box>(className(), name, parent);
    myClone->_cloned = true;

    // Loop through input and output ports
    // The ports in the clones have been created by Input and Output and with their defaults settings
    // but they miss any assignments from the boxscript
    auto origPort = _portsInOrder.begin();
    for (auto clonedPort = myClone->_portsInOrder.begin();
         clonedPort != myClone->_portsInOrder.end();
         ++origPort, ++clonedPort)
    {
        Expression expr = (*origPort)->expression();
         (*clonedPort)->equals( expr.setParent(*clonedPort) );
    }

    // Loop through auxiliary ports, which follows input and output ports
    for (; origPort != _portsInOrder.end(); ++origPort)
    {
        Expression expr = (*origPort)->expression();
        Port *clonedAuxPort = new Port((*origPort)->name(), PortType::Auxiliary, myClone);
        clonedAuxPort->equals( expr.setParent(clonedAuxPort) );
    }

    return myClone;
}

Box* Box::cloneFamily(QString name, Box *parent) {
    // First clone me then clone my children
    Box *myClone = clone(name, parent);
    for (Box *child : findMany<Box*>("./*"))
        child->cloneFamily(child->objectName(), myClone);
    return myClone;
}

bool Box::isCloned() const {
    return _cloned;
}

void Box::debug(bool on) {
    _debugOn = on;
}

bool Box::debug() {
    return _debugOn;
}

void Box::toText(QTextStream &text, QString options, int indentation) const {
    if (options.isEmpty())
        options = "ioa";
    bool writeOverriddenInputs = options.contains("I"),
         writeAllInputs        = options.contains("i") && !writeOverriddenInputs,
         writeOutputs          = options.contains("o"),
         writeAux              = options.contains("a");

    QString fill;
    fill.fill(' ', indentation);

    text << fill << className() << " " << objectName() << " { //" << order() << "\n";

    for (Port *port : _portsInOrder) {
        bool doWrite;
        switch (port->type()) {
        case PortType::Input:
            doWrite = writeAllInputs || (writeOverriddenInputs && port->isValueOverridden());
            break;
        case PortType::Output:
            doWrite = writeOutputs;
            break;
        case PortType::Auxiliary:
            doWrite = writeAux;
            break;
        }
        if (doWrite)
            port->toText(text, indentation+2);
    }

    Box *me = const_cast<Box*>(this);
    for (Box *box : me->findMany<Box*>("./*"))
        box->toText(text, options, indentation+2);

    text << fill << "}\n";
}

QString Box::toText(QString options) const {
    QString s;
    QTextStream text(&s);
    toText(text, options, 0);
    return s;
}

}
