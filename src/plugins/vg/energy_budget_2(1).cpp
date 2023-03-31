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

QString Radiation::columnLabels() {
    stringstream str;
    str << setw(8) << "Layer"
        << setw(5) << "a"
        << setw(5) << "r"
        << setw(5) << "t"
        << setw(5) << "a_"
        << setw(5) << "r_"
        << setw(5) << "t_"
        << setw(6) << "E"
        << setw(6) << "E_"
        << setw(6) << "F"
        << setw(6) << "F_"
        << setw(6) << "A"
        << setw(6) << "A_";
    return QString::fromStdString(str.str());
}

std::ostream& operator<<(ostream& os, const Radiation& x) {
    os << fixed;
    os << setw(8) << x.name.toStdString()
       << setprecision(2)
       << setw(5) << *x.a
       << setw(5) << *x.r
       << setw(5) << *x.t
       << setw(5) << *x.a_
       << setw(5) << *x.r_
       << setw(5) << *x.t_
       << setprecision(1)
       << setw(6) << x.E
       << setw(6) << x.E_
       << setw(6) << x.F
       << setw(6) << x.F_
       << setw(6) << x.A
       << setw(6) << x.A_;
    return os;
}

std::ostream& operator<<(std::ostream& os, const RadiationBudget& x) {
    os << Radiation::columnLabels().toStdString() << endl;
    for (auto it=x.layers.begin(); it!=x.layers.end(); ++it)
        os << *it << "\n";
    return os;
}

QString Heat::columnLabels() {
    stringstream str;
    str << setw(6) << "T";
    return QString::fromStdString(str.str());
}

std::ostream& operator<<(std::ostream& os, const Heat& x) {
    os << fixed
       << setprecision(1)
       << setw(6) << *x.T;
    return os;
}

std::ostream& operator<<(std::ostream& os, const RadiationAndHeat& x) {
    os << x.radiation << x.heat;
    return os;
}

std::ostream& operator<<(std::ostream& os, const RadiationAndHeatBudget& x) {
    os << RadiationAndHeat::columnLabels().toStdString() << endl;
    for (auto it=x.layers.begin(); it!=x.layers.end(); ++it)
        os << *it << "\n";
    return os;
}

QString RadiationAndHeat::columnLabels() {
    return Radiation::columnLabels() + Heat::columnLabels();
}

void RadiationBudget::initiateRadiation() {
    for (auto it=layers.begin(); it!=layers.end(); ++it) {
        it->F  = it->E;
        it->F_ = it->E_;
    }
}

void RadiationBudget::distributeRadDown() {
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

void RadiationBudget::distributeRadUp() {
    auto cur = layers.end() - 1;
    for (auto next = cur-1; cur != layers.begin(); --cur, --next) {
        // Correct absorption and transmission of this layer and layer above
        // for reflections ad infinitum between the two layers
        double
            k = 1. - (*next->r_) * (*cur->r),
            ah_ = (*next->a_)/k,
            th_ = (*next->t_)/k,
            ah  = (*cur->a)/k * (*next->r_),
            th  = (*cur->t)/k * (*next->r_),
            // Absorbed by layer above
            absorbed_    = ah_ * cur->F_,
            // Transmitted through layer above
            transmitted_ = th_ * cur->F_,
            // Absorbed by this layer by reflection from above
            absorbed     = ah  * cur->F_,
            // Transmitted through this layer by reflection from above
            transmitted  = th  * cur->F_;
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

void RadiationBudget::distributeRadiation() {
    const double precision = 1e-6;
    iterations = 0;
    double residual;
    initiateRadiation();
        do {
        ++iterations;
        distributeRadDown();
        distributeRadUp();
        residual = 0.;
        for (auto it=layers.begin(); it!=layers.end(); ++it)
            residual += it->F + it->F_;
    } while (residual > precision);
}

QVector<double> RadiationBudget::A() const {
    QVector<double> Avalues;
    for (auto it=layers.begin(); it!=layers.end(); ++it)
        Avalues << it->A;
    return Avalues;
}

QVector<double> RadiationBudget::A_() const {
    QVector<double> A_values;
    for (auto it=layers.begin(); it!=layers.end(); ++it)
        A_values << it->A_;
    return A_values;
}

void RadiationAndHeatBudget::updateLwEmission() {
    for (auto it=layers.begin(); it!=layers.end(); ++it) {
        it->radiation.E  = Sigma*(*it->radiation.a_)*p4(*it->heat.T + T0);
        it->radiation.E_ = Sigma*(*it->radiation.a )*p4(*it->heat.T + T0);
    }
}

void Budget::transferSwAbsorption() {
    auto swIt = sw.layers.begin();
    QVector<RadiationAndHeat>::iterator radAndHeatIt = radAndheat.layers.begin();
    for (; swIt != sw.layers.end(); ++swIt, ++radAndHeatIt) {
        radAndHeatIt->radiation.A  = swIt->A;
        radAndHeatIt->radiation.A_ = swIt->A_;
    }
}

//void Layers::updateTemperatureByRadiation(const Layers &sw, double sub_dt) {
//    for (auto it=layers.begin(), it2=sw->begin(); it!=layers.end(); ++it, ++it2) {
//        it->T += (it->netRadiation() + ite->netRadiation()) / (it->C) * sub_dt;
//    }
//}

//void Layers::updateInMinorSteps(double dt, int n) {
//    double sub_dt = dt/n;

//}

} //namespace

