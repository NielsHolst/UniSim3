/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <iomanip>
#include <sstream>
#include <base/phys_math.h>
#include "energy_budget_2.h"

using namespace std;
using namespace phys_math;

namespace vg {

EnergyBudget2::EnergyBudget2(QString name, Box *parent)
    : Box(name, parent)
{
}

//QString EnergyBudget2::Radiation::columnLabels() {
//    stringstream str;
//    str << setw(8)
//       << "Layer"
//       << setw(5)
//       << "a" << "r" << "t" << "a_" << "r_" << "t_"
//       << setw(6)
//       << "E" << "E_" << "F" << "F_" << "A" << "A_";
//    return QString::fromStdString(str.str());
//}

//std::ostream& operator<<(ostream& os, const EnergyBudget2::Radiation& x) {
//    os << setw(8)
//       << x.name.toStdString()
//       << setprecision(2) << setw(5)
//       << *x.a << *x.r << *x.t << *x.a_ << *x.r_ << *x.t_
//       << setprecision(1) << setw(6)
//       << x.E << x.E_ << x.F << x.F_ << x.A << x.A_;
//    return os;
//}

void EnergyBudget2::Layers::distributeRadDown() {
    auto cur = layers.begin();
    for (auto next = cur+1; next != layers.end(); ++cur, ++next) {
        // Correct absorption and transmission of this layer and layer below
        // for reflections ad infinitum between the two layers
        double
            k = 1. - (*next->r) * (*cur->r_),
            ah  = (*next->a)/k,
            th  = (*next->t)/k,
            ah_ = (*cur->a_)/k * (*next->r),
            th_ = (*cur->t_)/k * (*next->r),
            // Absorbed by layer below
            absorbed     = ah  * cur->F,
            // Transmitted through layer below
            transmitted  = th  * cur->F,
            // Absorbed by this layer by reflection from below
            absorbed_    = ah_ * cur->F,
            // Transmitted through this layer by reflection from below
            transmitted_ = th_ * cur->F;
        // Update layer below
        next->A += absorbed;
        next->F += transmitted;
        // Update this layer
        cur->A_ += absorbed_;
        cur->F_ += transmitted_;
        // Downwards flow from this layer has been spent
        cur->F = 0;
    }
}

void EnergyBudget2::Layers::distributeRadUp() {
    auto cur = layers.end() - 1;
    for (auto next = cur-1; next != layers.end(); --cur, --next) {
        // Correct absorption and transmission of this layer and layer above
        // for reflections ad infinitum between the two layers
        double
            k = 1. - (*next->r) * (*cur->r_),
            ah_ = (*next->a_)/k,
            th_ = (*next->t_)/k,
            ah  = (*cur->a)/k * (*next->r_),
            th  = (*cur->t)/k * (*next->r_),
            // Absorbed by layer above
            absorbed     = ah_ * cur->F_,
            // Transmitted through layer above
            transmitted  = th_ * cur->F_,
            // Absorbed by this layer by reflection from above
            absorbed_    = ah  * cur->F_,
            // Transmitted through this layer by reflection from above
            transmitted_ = th  * cur->F_;
        // Update layer above
        next->A_ += absorbed_;
        next->F_ += transmitted_;
        // Update this layer
        cur->A   += absorbed;
        cur->F   += transmitted;
        // Upwards flow from this layer has been spent
        cur->F_ = 0;
    }
}

void EnergyBudget2::Layers::distributeRadiation() {
    const double precision = 1e-6;
    iterations = 0;
    double residual;
    for (auto it=layers.begin(); it!=layers.end(); ++it) {
        it->F  = it->E;
        it->F_ = it->E_;
    }
    do {
        ++iterations;
        distributeRadDown();
        distributeRadUp();
        residual = 0.;
        for (auto it=layers.begin(); it!=layers.end(); ++it)
            residual += it->F + it->F_;
    } while (residual > precision);
}

//void Layers::updateLwEmission() {
//    for (auto it=layers.begin(); it!=layers.end(); ++it) {
//        it->E  = Sigma*(*it->a_)*p4(*it->T + T0);
//        it->E_ = Sigma*(*it->a )*p4(*it->T + T0);
//    }
//}

//void Layers::updateTemperatureByRadiation(const Layers &sw, double sub_dt) {
//    for (auto it=layers.begin(), it2=sw->begin(); it!=layers.end(); ++it, ++it2) {
//        it->T += (it->netRadiation() + ite->netRadiation()) / (it->C) * sub_dt;
//    }
//}

//void Layers::updateInMinorSteps(double dt, int n) {
//    double sub_dt = dt/n;

//}

} //namespace

