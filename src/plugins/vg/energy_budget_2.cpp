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

QString Layer::columnLabels() {
    stringstream str;
    str << setw(8) << "Layer"
        << setw(5) << "a"
        << setw(5) << "r"
        << setw(5) << "t"
        << setw(5) << "a_"
        << setw(5) << "r_"
        << setw(5) << "t_"
        << setw(6) << "T"
        << setw(6) << "E"
        << setw(6) << "E_"
        << setw(6) << "F"
        << setw(6) << "F_"
        << setw(6) << "A"
        << setw(6) << "A_"
        << setw(6) << "U"
        << setw(6) << "U_"
        << setw(6) << "H"
        << setw(6) << "H_";
    return QString::fromStdString(str.str());
}

std::ostream& operator<<(ostream& os, const Layer& x) {
    os << fixed
       << setw(8) << x.name.toStdString()
       << setprecision(2)
       << setw(5) << *x.a
       << setw(5) << *x.r
       << setw(5) << *x.t
       << setw(5) << *x.a_
       << setw(5) << *x.r_
       << setw(5) << *x.t_
       << setprecision(1)
       << setw(6) << x.T
       << setw(6) << x.E
       << setw(6) << x.E_
       << setw(6) << x.F
       << setw(6) << x.F_
       << setw(6) << x.A
       << setw(6) << x.A_
       << setw(6) << x.U
       << setw(6) << x.U_
       << setw(6) << x.H
       << setw(6) << x.H_;
    return os;
}

QString Volume::columnLabels() {
    stringstream str;
    str << setw(8) << "Layer"
        << setw(6) << "T"
        << setw(7) << "C"
        << setw(6) << "H"
        << setw(6) << "V";
    return QString::fromStdString(str.str());
}

std::ostream& operator<<(std::ostream& os, const Volume& x) {
    os << fixed
       << setw(8) << x.name.toStdString()
       << setprecision(1)
       << setw(6) << x.T
       << setw(7) << *x.C
       << setw(6) << x.H
       << setw(6) << x.V;
    return os;
}

QString Water::columnLabels() {
    stringstream str;
    str << setw(8) << "Layer"
        << setw(7) << "RH"
        << setw(12) << "Tr"
        << setw(12) << "Cn"
        << setw(12) << "Mv";
    return QString::fromStdString(str.str());
}

std::ostream& operator<<(std::ostream& os, const Water& x) {
    os << fixed
       << setw(8) << x.name.toStdString()
       << setprecision(2)
       << setw(7) << x.RH
       << setprecision(8)
       << setw(12) << x.Tr
       << setw(12) << x.Cn
       << setw(12) << x.Mv;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Budget& x) {
    os << Layer::columnLabels().toStdString() << endl;
    for (auto it=x.layers.begin(); it!=x.layers.end(); ++it)
        os << *it << endl;
    os << endl << Volume::columnLabels().toStdString() << endl;
    for (auto it=x.volumes.begin(); it!=x.volumes.end(); ++it)
        os << *it << endl;
    os << endl << Water::columnLabels().toStdString() << endl;
    for (auto it=x.waters.begin(); it!=x.waters.end(); ++it)
        os << *it << endl;
    return os;
}

void Budget::transferEmission() {
    for (auto it=layers.begin(); it!=layers.end(); ++it) {
        it->F  = it->E;
        it->F_ = it->E_;
    }
}

void Budget::distributeRadDown() {
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

void Budget::distributeRadUp() {
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

void Budget::distributeRadiation() {
    const double precision = 1e-6;
    iterations = 0;
    double residual;
    transferEmission();
        do {
        ++iterations;
        distributeRadDown();
        distributeRadUp();
        residual = 0.;
        for (auto it=layers.begin(); it!=layers.end(); ++it)
            residual += it->F + it->F_;
    } while (residual > precision);
}

void Budget::updateLwEmission() {
    for (auto it=layers.begin(); it!=layers.end(); ++it) {
        if (it->name != "Light" && it->name != "Heating") {
            it->E  = Sigma*(*it->a_)*p4(it->T + T0);
            it->E_ = Sigma*(*it->a )*p4(it->T + T0);
        }
    }
}

void Budget::transferAbsorption(const Budget &b) {
    auto itb = b.layers.begin();
    for (auto it=layers.begin(); it!=layers.end(); ++it, ++itb) {
        it->A  = itb->A;
        it->A_ = itb->A_;
    }
}

void Budget::transferConvection(const Budget &b) {
    auto itl = b.layers.begin();
    for (auto it=layers.begin(); it!=layers.end(); ++it, ++itl) {
        it->H  = itl->H;
        it->H_ = itl->H_;
    }
    auto itv = b.volumes.begin();
    for (auto it=volumes.begin(); it!=volumes.end(); ++it, ++itv) {
        it->H  = itv->H;
    }
}

namespace {
    double delta_vent(double yin0, double yout, double v, double dt) {
      return (yout-yin0)*(1 -exp(-v/3600*dt));
    }
    double dE_in(double Tin0, double Tout, double v, double dt) {
      return delta_vent(Tin0, Tout, v, dt)*4780/dt;
    }
}

void Budget::updateAdvectionHeat(double ventilationRate, double sub_dt) {
    Volume
        &outdoors(volumes[0]),
        &indoors (volumes[1]);
    double flux = dE_in(indoors.T, outdoors.T, ventilationRate, sub_dt);
    outdoors.V = -flux;
    indoors.V  =  flux;
}

void Budget::updateAdvectionHumidity(double ventilationRate, double sub_dt) {
    Volume
        &outdoorsV(volumes[0]),
        &indoorsV (volumes[1]);
    Water
        &outdoorsW(waters[0]),
        &indoorsW (waters[1]);
    // Compute absolute humidity flux
    double indoorsAh  = ahFromRh(indoorsV.T,  indoorsW.RH),
           outdoorsAh = ahFromRh(outdoorsV.T, outdoorsW.RH),
           fluxAh = dE_in(indoorsAh, outdoorsAh, ventilationRate, sub_dt);
    // Update water flux
    outdoorsW.Mv = -fluxAh;
    indoorsW.Mv  =  fluxAh;
    // Update relative humidity
    indoorsW.RH = rhFromAh(indoorsV.T, indoorsAh + fluxAh);
}

void Budget::updateTemperatureByRadiation(double sub_dt) {
    for (auto it=layers.begin(); it!=layers.end(); ++it) {
        it->T += it->netRadiation() / (*it->C) * sub_dt;
        it->A  =
        it->A_ = 0.;
    }
}

void Budget::updateTemperature(double sub_dt) {
    for (auto it=layers.begin(); it!=layers.end(); ++it) {
        if (it->name == "Plant") {
            updatePlantTemperature();
        }
        else {
            const double heatFlux = it->netRadiation() + it->H + it->H_;
            it->T += heatFlux / (*it->C) * sub_dt;
        }
        it->A  =
        it->A_ = 0.;
    }
    Volume &indoors( volumes[1]);
    indoors.T += (indoors.H + indoors.V) / (*indoors.C) * sub_dt;
}

double Budget::ri(double RH, double CO2) {
    const double g0 = 0.1, g1 = 1.64, Pn = 2;
    return 1./(g0 + g1*RH/100*Pn/CO2);
}

double Budget::Tplant (double Arad, double Tin, double rh, double ri, double re) {
  const double Lai = 1.9;
  double
    a = (ri+re)/2/Lai/RhoAir/CpAir*Arad - 1/Psychr*(svp(Tin) - vpFromRh(Tin, rh)),
    b = 1 + svpSlope(Tin)/Psychr + ri/re;
  return Tin + a/b;
}

double Budget::transpiration(double Arad, double Tin, double rh, double ri, double re) {
    const double Lai = 1.9;
  double
      a = svpSlope(Tin)/Psychr*Arad + 2*Lai*RhoAir*CpAir/Psychr/re*(svp(Tin) - vpFromRh(Tin, rh)),
      b = LHe*(1 + svpSlope(Tin)/Psychr + ri/re);
  return a/b;
}

double Budget::glassCondensationRate() {
    const double g = 2e-3;
    Layer
        &glass(layers[1]);
    Volume
        &indoorsV(volumes[1]);
    Water
        &indoorsW(waters[1]);
    // At glass
    double
        glassSah  = sah(glass.T),
        indoorsAh = ahFromRh(indoorsV.T, indoorsW.RH);
    return std::max(g*(indoorsAh - glassSah), 0.);
}

void Budget::updatePlantTemperature(double co2) {
    Layer &plant(layers[4]);
    Volume &indoorsV(volumes[1]);
    Water &indoorsW(waters[1]);
    plant.T = Tplant(plant.netRadiation(), indoorsV.T, indoorsW.RH, ri(indoorsW.RH, co2), 218);
}

QVector<double> Budget::A() const {
    QVector<double> Avalues;
    for (auto it=layers.begin(); it!=layers.end(); ++it)
        Avalues << it->A;
    return Avalues;
}

QVector<double> Budget::A_() const {
    QVector<double> A_values;
    for (auto it=layers.begin(); it!=layers.end(); ++it)
        A_values << it->A_;
    return A_values;
}

QVector<double> Budget::T() const {
    QVector<double> Tvalues;
    for (auto it=layers.begin(); it!=layers.end(); ++it)
        Tvalues << it->T;
    return Tvalues;
}

QVector<double> Budget::Tv() const {
    QVector<double> Tvalues;
    for (auto it=volumes.begin(); it!=volumes.end(); ++it)
        Tvalues << it->T;
    return Tvalues;
}

void Budget::updateConvection() {
    Volume
        &outdoors(volumes[0]),
        &indoors( volumes[1]),
        &soil   ( volumes[2]);
    for (auto it = layers.begin() + 2; it!=layers.end()-1; ++it) {
        if (it->U  > 0.)
            indoors.H -= it->H  += it->U *(indoors.T - it->T);
        if (it->U_ > 0.)
            indoors.H -= it->H_ += it->U_*(indoors.T - it->T);
    }
    Layer
        &glass(layers[1]),
        &floor(layers[6]);
    outdoors.H -= glass.H  = glass.U  *(outdoors.T - glass.T);
    indoors.H  -= glass.H_ = glass.U_ *(indoors.T  - glass.T);
    indoors.H  -= floor.H  = floor.U  *(indoors.T  - floor.T);
    soil.H     -= floor.H_ = floor.U_ *(soil.T     - floor.T);
}

void Budget::updateHumidity(double ventilationRate, double co2, double sub_dt) {
    Layer
        &plant(layers[4]);
    Volume
        &indoorsV(volumes[1]);
    Water
        &indoorsW (waters[1]);
    indoorsW.Tr = transpiration(plant.netRadiation(), indoorsV.T, indoorsW.RH, ri(indoorsW.RH, co2), 218);
    indoorsW.Cn = glassCondensationRate();
    updateAdvectionHumidity(ventilationRate, sub_dt);
}

} //namespace

