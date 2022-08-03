/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "leaf_light_response_processes_deprecated.h"

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(LeafLightResponseProcessesDeprecated)

const double O2i = 210;     // [G94, p.80] O2 partial pressure inside stomata [ppm], [mbar], [ml/l]

LeafLightResponseProcessesDeprecated::LeafLightResponseProcessesDeprecated(QString name, Box *parent)
	: Box(name, parent)
{
    help("models various processes of leaf light response");
    Input(Tleaf).imports("../../../temperature[value]",CA).unit("oC");
    Input(rhoChl).equals(0.45).help("Chlorophyll density").unit("unit?");
    Input(concEnzyme).equals(87.).help("Enzyme concentration").unit("unit?");
    Input(KC25).equals(310.).help("Rubisco carboxylation at 25C").unit("my bar");
    Input(KO25).equals(155.).help("Rubisco oxygenation at 25C").unit("mbar");
    Output(VCmax).help("Maximum carboxylation rate ").unit("mu mol e-/m2/s");
    Output(Jmax).help("Maximum photosynthetic capacity").unit("mg CO2/m2/s");
    Output(gamma).help("CO2 compensation concentration").unit("ppm or mu bar?");
    Output(KM).help("Rubisco carboxylation").unit("mg CO2/m3");
}

void LeafLightResponseProcessesDeprecated::reset() {
    update();
}

void LeafLightResponseProcessesDeprecated::update() {
    // Update basics
    T25 = 25. + T0;
    TleafK = Tleaf + T0;
    x25 = (Tleaf-25.)/(TleafK*R*T25); // [F80, p.83] intermediate variable [mol/J]
    rhoCo2T = RhoCo2*T0/TleafK;      // [gaslaw] density CO2 at Tleaf [kg/m3], [mg/ml]

    // Michaelis-Menten
    const double EC = 59356,    // KC Rubisco carboxylation
                 EO = 35948;    // KO Rubisco oxygenation
    KC = KC25*exp(EC*x25);      // [F80, p.83] M-M constant Rubisco carboxylation (CO2) [ubar]
    KO = KO25*exp(EO*x25);      // [F80, p.83] M-M constant Rubisco oxygenation (O2) [mbar]

    // Update outputs
    VCmax = maxCarboxylationRate();
    Jmax = maxPhotosyntheticCapacity();
    gamma = co2CompensationConcentration();
    KM = RubiscoCarboxylation();
}

double LeafLightResponseProcessesDeprecated::maxCarboxylationRate() {
    const double EVC = 58520,               // maximum carboxylation rate
                 tc = 2.5;                  // [F80, p.89] (kc) turnover number of RuP2 carboxylase [1/s]
    double VCmax25 = rhoChl*tc*concEnzyme;  // [F80, p.84] maximum carboxylation rate [umol CO2/m2/s]
    return VCmax25*exp(EVC*x25);            // [F80, p.83] maximum carboxylation rate [umol e-/m2/s]
}

double LeafLightResponseProcessesDeprecated::maxPhotosyntheticCapacity() {
    // [F80, p.88-89] constants for optimum curve temperature dependent electron transport rate
    const double S = 710,                   // [J/mol/K]
                 H = 220000,                // [J/mol]
                 Jmax25 = 467*rhoChl;       // [F80, p.84] maximum electron transport rate
                                            // (on chlorofylbasis) [umol e-/m2/s]
    const double EJ = 37000;                // Jmax maximum electron transport rate
    double D1 = 1 + exp((S - H/TleafK)/R);  // [F82, erratum] intermediate variable [-]
    double D0 = 1 + exp((S - H/T25)/R);     // [G94, p.VII-5] intermediate variable [-]
    double D = D1/D0;                       // [G94, p.VII-5] intermediate variable [-]

    double Jmax = Jmax25*exp(EJ*x25)/D;     // [F80, p.84] maximum electron transport rate
                                            // (on chlorofylbasis) [umol e-/m2/s]
    return MCo2/4*Jmax;                     // [??  (2.31 at 25 oC)] maximum endogenous
                                            // photosynthetic capacity [mg CO2/m2/s]
}

double LeafLightResponseProcessesDeprecated::co2CompensationConcentration() {
    const double VOC = 0.21;        // [F80, p.81] VOC=VOmax/VCmax=ko/kc=constant
    return KC*O2i*VOC/(2*KO); // [F80, p.85] CO2 compensation concentration in absence of dark respiration [ppm], [ubar]
}

double LeafLightResponseProcessesDeprecated::RubiscoCarboxylation() {
    return KC*(1+O2i/KO)*rhoCo2T;     // [G94, p.80] M-M constant Rubisco carboxylation (CO2) [mg CO2/m3]
}

} //namespace


