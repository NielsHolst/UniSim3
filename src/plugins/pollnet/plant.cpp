/* Copyright 2017-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Kayla Sale, University of Arizona [kayla.sale at email.arizona.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "plant.h"

using namespace base;

namespace pollnet {

PUBLISH(Plant)

Plant::Plant(QString name, QObject *parent)
    : Box(name, parent)
{
    Input(initial_R).imports("random/plant/initial_R[value]").help("Initial biomass of floral rewards.");
    Input(initial_V).imports("random/plant/initial_V[value]").help("Initial biomass of vegetative portion of plant population.");

    Input(m).imports("random/plant/m[value]").help("body mass of plant");
    Input(x).help("Metabolic rate of plant");

    Input(loss_floral).help("Sum consumption rate of pollinators on floral rewards; computed by mediator");
    Input(reproductive_services);

    Input(Vsum).help("Total plant vegetative biomass; computed by mediator");

    Input(benefit_coefficient).equals(100).help("Sets steep-ness of saturating benefit accrual functional response.");
    Input(s).equals(2).help("Self-limitation rate of floral rewards production.");
    Input(beta).equals(1).help("Producton rate of floral rewards.");
    Input(K).equals(5).help("Plant community-wide carrying capcity.");
    Input(kappa).imports("random/plant[kappa]").help("Cost of producing floral rewards in terms of vegetative biomass.");

    Output(R).help("Biomass of floral rewards of plant population.");
    Output(V).help("Biomass of vegetative portion of plant population.");
}

void Plant::reset() {
    // Allometry
    x = pow(m, 0.75);
    r = x/0.138;

    R = initial_R;
    V = initial_V;
}

double Plant::compute_floral_growth() {
    double floral_growth = beta*V - s*R;
    return floral_growth; // change to just "return beta*V - s*R"
}


double Plant::compute_benefit_accrual() {
    double benefit_accrual = reproductive_services/(benefit_coefficient+reproductive_services);
    return benefit_accrual; // change to just "return reproductive_service/(benefit_coefficient+reproductive_service)"
}

double Plant::compute_K_effect() {
    double K_effect = 1 - (Vsum/K);
    return K_effect;
}

double Plant::compute_dRdt() {
    return compute_floral_growth() - loss_floral;
}

double Plant::compute_dVdt() {
    double K_effect = compute_K_effect();
    double benefit_accrual = compute_benefit_accrual();
    double floral_growth = compute_floral_growth();

    double dVdt = K_effect*r*V*benefit_accrual - x*V - kappa*floral_growth;

    return dVdt;
}

void Plant::update() {
    // compute_new_R, compute_new_V
    const double dt = 0.001;
    double dRdt = compute_dRdt();
    double dVdt = compute_dVdt();

    R = R + dRdt * dt;
    V = V + dVdt * dt;

    if(R <= 0) {
        R = 0;
    }
    if(V <= 0) {
        V = 0;
        R = 0;
    }
}

}
