/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/unique_names.h>
#include "average.h"

using namespace base;

namespace boxes {

PUBLISH(Average)

Average::Average(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes average through simulation steps");
    Input(ports).help("Ports to average");
    Input(isActive).imports("selector[isActive]");
    Output(count).help("Number of values gone into average");
}

void Average::amend() {
    // Find all ports in output
    for (auto port : ports.findMany<Port*>(this)) {
       _ports << port;
       _values << port->valuePtr<Value>();
    }

    // Find unique names to use for naming output ports
    QStringList names = UniqueNames(_ports).resolved();

    // Allocate vectors to collect averages
    _averages.resize(names.size());

    // Create output ports
    int i = 0;
    for (auto name : names)
        NamedOutput(name, _averages[i++]);
}

void Average::reset() {
    int i = 0;
    for (double &avg : _averages)
        avg = _values.at(i++)->as<double>();
    count = 0;
    _zeroNext = false;
}

void Average::update() {
    if (_zeroNext)
        zeroOut();
    int i = 0;
    for (double &avg : _averages)
        avg += _values.at(i++)->as<double>();
    ++count;
    if (isActive) {
        for (double &avg : _averages)
            avg /= count;
        _zeroNext = true;
    }
}

void Average::zeroOut() {
    for (double &avg : _averages)
        avg = 0.;
    count = 0;
    _zeroNext = false;
}

} //namespace

