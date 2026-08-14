/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mohammad Ali Mirhosseini, Tarbiat Modares University, Iran [mirhosseini1989 at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "Nesidiocoris.h"
using namespace base;

namespace tuta {

PUBLISH(Nesidiocoris)

Nesidiocoris::Nesidiocoris(QString name, QObject *parent)
    : Box(name, parent) {
    help("computes linear day-degrees");
    Input(P).equals(0).help("a constant parameter");
    Input(m).equals(0).help("a constant parameter");
    Input(n).equals(0).help("a constant parameter");
    Input(Tu).equals(0).help("Upper temperature");
    Input(T0).equals(8).help("Lower temperature threshold");
    Input(T).imports("weather[Tavg]");
    Input(timeStepDays).equals(1).help("Length of a time steps in days");
    Input(resetTotal).equals(false).help("Reset total to zero?");
    Input(isTicking).equals(true).help("Are day-degrees being computed? Else step will be zero");
    Output(step).help("Increment in day-degrees");
    Output(total).help("Total day-degrees since reset");
}

void Nesidiocoris::reset() {
    if (T >= 60) {
        QString msg{"T is very high"};
        ThrowException(msg).context(this);
    }
    }

void Nesidiocoris::update() {

    double x = (T-T0)/(Tu-T0);
    if (!isTicking)
        step = 0.;
    else if (T <= T0)
        step = 0.;
    else if (x > 1)
        step = 0.;
    else
        step = P*(pow(x,n))*(pow((1-x),m));

        step *= timeStepDays;

    if (resetTotal)
        total = 0;
    total += step;
}

} //namespace

