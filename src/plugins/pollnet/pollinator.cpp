/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Kayla Sale, University of Arizona [kayla.sale at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "pollinator.h"

using namespace base;

namespace pollnet {

PUBLISH(Pollinator)

Pollinator::Pollinator(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initial_B).imports("random/pollinator/initial_B[value]").help("Initial biomass of pollinator population.");
    Input(m).imports("random/pollinator/m[value]").help("Body mass of pollinator in __ units.");
    Input(is_vertebrate).imports("random/pollinator/is_vertebrate[value]").help("Boolean variable, TRUE if vertebrate.");

    Input(CR).help("Consumption rate; computed by mediator");

    Output(x).help("Metabolic rate of pollinator");
    Output(y).help("Max. consumption rate");
    Output(B).help("Biomass of pollinator population.");

}

void Pollinator::reset() {
    // Allometry
    x = is_vertebrate ? 0.88*pow(m, 0.75) : 0.314*pow(m, 0.75);

    y = is_vertebrate ? 4 : 8;

    B = initial_B;
}


double Pollinator::compute_dBdt() {
    return CR-x*B;
}

void Pollinator::update() {
    // compute_new_B
    const double dt = 0.001;
    double dBdt = compute_dBdt();
    B = B +  dBdt * dt;

    if(B <= 0) {
        B = 0;
    }
}

}
