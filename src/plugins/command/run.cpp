/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <QTime>
#include <base/box.h>
#include <base/command_help.h>
#include <base/computation.h>
#include <base/dialog.h>
#include <base/environment.h>
#include <base/exception.h>
#include <base/path.h>
#include <base/port.h>
#include <base/publish.h>
#include "load.h"
#include "run.h"

using namespace base;

namespace command {

PUBLISH(run)
HELP(run, "run", "runs simulation")

run::run(QString name, Box *parent)
    : Command(name, parent)
{
}

void run::doExecute() {
    if (_args.size() > 2)
        ThrowException("Command 'run' takes at most one argument");
    dialog().resetErrorCount();
    doLoad();
    if (dialog().errorCount() == 0)
        doRun();
}

void run::doLoad() {
    QStringList com;
    com << "load";
    switch (_args.size()) {
    case 1:
        com << environment().latestLoadArg();
        break;
    case 2:
        com << _args[1];
    }
    Command::submit(com);
}

void run::doRun() {
    _root = Box::root();
    if (!_root) {
        dialog().error("Nothing to run");
        return;
    }
    _root->run();

    const Port *hasErrorPort = _root->peakPort("hasError");
    bool noError = !(hasErrorPort && hasErrorPort->value<bool>());

    QString msg = noError ? "Finished" : "Interrupted";
    QString msgIteration = message("iteration", "iterations", "iteration");
    msg += messageTime();
    msg += msgIteration;
    if (msgIteration.isEmpty())
        msg += message((noError ? "finalStep" : "step"), "steps", "step");

    if (noError)
        dialog().information(msg);
    else
        dialog().error(msg + errorMessage());
    Computation::changeStep(Computation::Step::Ready);
}

QString run::messageTime() const {
    const Port *port = _root->peakPort("executionTime");
    if (!port)
        return "";
    int dt = port->value<int>();
    QString units = "msecs";
    if (dt > 5000) {
        dt /= 1000;
        units = "secs";
    }
    QString s{" after %1 %2"};
    return s.arg(dt).arg(units);
}

QString run::message(QString counter, QString total, QString in) const {
    Port *i = _root->peakPort(counter),
         *n = _root->peakPort(total);
    if (!i || !n || n->value<int>() == 1)
        return "";

    QString s = " in " + in + " %1/%2";
    return s. arg(i->value<int>()). arg(n->value<int>());
}

QString run::errorMessage() const {
    const Port *port = _root->peakPort("errorMsg");
    return port ? ("\n" + port->value<QString>()) : QString();
}

}
