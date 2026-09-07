/* Copyright 2025-2026 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Liyan Xie, Aarhus University [liyan.xie at agro.au.dk] and 
** Anne Dahl Svendsen, Aarhus University [anne.svendsen at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/exception.h>
#include <base/publish.h>
#include "logistic_growth.h"

using namespace base;

namespace collembola {

PUBLISH(LogisticGrowth)

LogisticGrowth::LogisticGrowth(QString name, Box *parent)
    : Box(name, parent)
{
    Doc("models logistic growth");
    Input(r).equals(0.02).unit("per day").help("Exponential growth rate");
    Input(K).equals(50.).unit("per km2").help("Carrying capacity");
    Input(N0).equals(3.5).unit("per km2").help("Initial density");
    Input(dt).equals(1.).unit("days").help("Simulation time step");
    Output(N).unit("per km2").help("Density");
    Output(dN).unit("per km2").help("Density increment");
}

void LogisticGrowth::reset() {
    // Don't apply increment at start
    N = N0;
    dN = increment();
}

void LogisticGrowth::update() {
    dN = increment();
    N += dN;
}

double LogisticGrowth::increment() const {
    if (K <= 0)
        ThrowException("Carrying capacity must be > 0").value(K);
    return r*dt*N*(1 - N/K);
}

} //namespace

