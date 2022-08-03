/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "loss_rate_total.h"
#include <cmath>
#include <base/publish.h>

using namespace base;

namespace PestTox {

PUBLISH (LossRateTotal)
	
LossRateTotal::LossRateTotal(QString name, Box *parent)
    : Box(name, parent)
{
    help("manages total loss rate");
    Input(amount).imports("compartments/crop[load]").help("Amount from which losses are taken");
    Input(path).equals("../potential/*[finite]").help("Path to loss rates (per day)");
    Output(fractionLost).help("Fraction lost calculated from loss rates on path (per day)");
    Output(amountLost).help("Amount lost (per day)");
}

void LossRateTotal::amend() {
    // Find input ports
    QVector<Port*> ports = findMany<Port*>(path);

    // Collect pointers to port values;
    _inputs.clear();
    for (Port *port : ports)
        _inputs << port->valuePtr<double>();

    // Create output ports
    _outputRates.resize(ports.size());
    _outputAmounts.resize(ports.size());
    double *outputRate = _outputRates.data(),
           *outputAmount = _outputAmounts.data();
    for (Port *port : ports) {
        QString name = port->parent()->objectName();
        NamedOutput(name+"Rate", *outputRate);
        NamedOutput(name+"Amount", *outputAmount);
        ++outputRate;
        ++outputAmount;
    }
}

void LossRateTotal::update() {
    // Compute total loss
    double remaining(1),
           totalInputs(0);
    for (const double *input : _inputs) {
        remaining *= (1. - *input);
        totalInputs += *input;
    }
    fractionLost = 1. - remaining;
    amountLost = fractionLost*amount;

    // Divide losses among rates
    double *outputRate = _outputRates.data(),
           *outputAmount = _outputAmounts.data();
    for (const double *input : _inputs) {
        double rate = *input/totalInputs * fractionLost;
        *outputRate++ = rate;
        *outputAmount++ = rate*amount;
    }
}


} //namespace

