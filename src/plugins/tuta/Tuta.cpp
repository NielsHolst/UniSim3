/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mohammad Ali Mirhosseini, Tarbiat Modares University, Iran [mirhosseini1989 at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "Tuta.h"
using namespace base;

namespace tuta {

PUBLISH(Tuta)

Tuta::Tuta(QString name, QObject *parent)
    : Box(name, parent) {
    help("computes linear day-degrees");
    Input(a0).equals(0).help("a constant parameter");
    Input(a1).equals(0).help("a constant parameter");
    Input(a2).equals(0).help("a constant parameter");
    Input(a3).equals(0).help("a constant parameter");
    Input(T0).equals(8).help("Lower temperature threshold");
    Input(T).imports("weather[Tavg]");
    Input(timeStepDays).equals(1).help("Length of a time steps in days");
    Input(resetTotal).equals(false).help("Reset total to zero?");
    Input(isTicking).equals(true).help("Are day-degrees being computed? Else step will be zero");
    Output(step).help("Increment in day-degrees");
    Output(total).help("Total day-degrees since reset");
}

void Tuta::reset() {
    if (T >= 50) {
        QString msg{"T is very high"};
        ThrowException(msg).context(this);
    }
    }

void Tuta::update() {
    if (!isTicking)
        step = 0.;
    else if (T <= T0)
        step = 0.;
    else
        step = a0 * (pow(T,3)) + a1 * (pow(T,2)) + a2 * (T) + a3;
    step *= timeStepDays;
    if (resetTotal)
        total = 0;
    total += step;
}

} //namespace

