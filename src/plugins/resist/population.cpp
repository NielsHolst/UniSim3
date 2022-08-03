/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Christian Nansen, University of California [chrnansen at ucdavis.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "population.h"

using namespace base;

namespace resist {

PUBLISH(Population)

Population::Population(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(Ninit).help("Initial population size");
    Input(K).help("Carrying capacity");
    Input(R).help("Net reproductive rate (per capita)");
    Input(survival).help("Survival [0;1]");
    Input(Ntotal).help("Total size of all populations");
    Input(steps).equals(1).help("Time steps per generation");
    Output(N).help("Population size");
}

void Population::reset() {
    N = Ninit;
}

void Population::update() {
    N = N*pow(survival, 1./steps)*exp(log(R)*(K-Ntotal)/K/steps);
}

}
