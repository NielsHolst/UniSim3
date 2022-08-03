/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "log_logistic.h"

using namespace base;

namespace io {

PUBLISH(LogLogistic)

LogLogistic::LogLogistic(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(dose).help("Dose for which response will be calculated. You can use this also as a push variable");
    Input(lower).help("Asymptotic response at low dose");
    Input(upper).equals(1.).help("Asymptotic response at high dose");
    Input(ED50).equals(10.).help("Dose giving average response");
    Input(slope).equals(2.).help("Slope of sigmoid curve");
    Output(value).help("Response at current dose");
}

void LogLogistic::initialize() {
}

void LogLogistic::reset() {
    value = f(dose = 0.);
}

void LogLogistic::update() {
    value = f(dose);
}

double LogLogistic::f(double z) {
    if (ED50 <= 0.)
        ThrowException("ED50 must have a positive value").value(ED50).context(this);
    if (slope == 0.) return (upper - lower)/2.;
    if (z == 0.) return (slope > 0.) ? lower : upper;
    return lower + (upper - lower)/(1. + pow((z/ED50), -slope));
}

} //namespace

