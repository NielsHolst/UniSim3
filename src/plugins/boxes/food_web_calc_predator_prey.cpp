/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include <base/test_num.h>
#include "food_web_calc_predator_prey.h"

using namespace base;
using namespace TestNum;

namespace boxes {

PUBLISH(FoodWebCalcPredatorPrey)

FoodWebCalcPredatorPrey::FoodWebCalcPredatorPrey(QString name, Box *parent)
    : FoodWebCalcBase(name, parent) {
    help("computes food web acquisition among Predator/Prey objects");
}

void FoodWebCalcPredatorPrey::amend() {
    FoodWebCalcBase::amend();
    // Allocate output buffers
    _Xloss.resize(_nResources, _nAttackers);
    _XlossTotal.resize(_nResources);
    _mortality.resize(_nResources);
    // Create predator-prey output ports
    for (int i=0; i<_nResources; ++i) {
        for (int j=0; j<_nAttackers; ++j) {
            QString suffix = "_" + _attackerNames.at(j) + "_" + _resourceNames.at(i);
            NamedOutput("loss" + suffix, _Xloss(i,j));
        }
    }
    // Create prey output ports
    for (int i=0; i<_nResources; ++i) {
        QString suffix = "_" + _resourceNames.at(i);
        NamedOutput("loss" + suffix, _XlossTotal[i]);
        NamedOutput("mortality" + suffix, _mortality[i]);
    }
}

void FoodWebCalcPredatorPrey::reset(    ) {
    FoodWebCalcBase::reset();
    _Xloss.fill(0.);
    _XlossTotal.fill(0.);
    _mortality.fill(0.);
}

void FoodWebCalcPredatorPrey::computeSearchRates() {
    // For every resource i, compute the search rates s_ij for all it attackers j by by eq. 8.
    for (int i=0; i<_nResources; ++i) {
        // Compute total search rate: si_
        double sum_aY(0);
        for (int j=0; j<_nAttackers; ++j)
            sum_aY += a(i,j)*Y(j);

        double si_ = 1. - exp(-sum_aY*timeStep);
        // Split search rate among predators: _s(i,j)
        for (int j=0; j<_nAttackers; ++j) {
            _s(i,j) = (sum_aY > 0.) ? a(i,j)*Y(j)/sum_aY*si_ : 0.;
        }
    }
}
void FoodWebCalcPredatorPrey::computeLosses() {
    // For every attacker j, compute the loss incured  on each of its resources i by eq. 13.
    _XlossTotal.fill(0);
    _mortality.fill(0.);
    for (int j=0; j<_nAttackers; ++j) {
        for (int i=0; i<_nResources; ++i) {
            _XlossTotal[i] += _Xloss(i,j) = (g(i,j) > 0.) ? S(i,j)/g(i,j) : 0.;
        }
    }
    // Compute total mortality on each ressource
    for (int i=0; i<_nResources; ++i) {
        // Use snapTo for numerical inaccuracy at small numbers
        snapTo(_XlossTotal[i], X(i));
        double mortality = (X(i) > 0.) ? _XlossTotal[i]/X(i) : 0.;
        snapTo(mortality, .1);
        _mortality[i] =  mortality;
     }
}

} //namespace

