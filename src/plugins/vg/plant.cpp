/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>

#include "plant.h"

using namespace base;
using namespace phys_math;
using namespace TestNum;

namespace vg {
	
PUBLISH(Plant)

Plant::Plant(QString name, Box *parent)
    : Box(name, parent),
      LayerAdjusted(name, parent)
{
    Input(k_sw).equals(0.7).unit("[0;1]").help("Short-wave extinction coefficient");
    Input(k_lw).equals(1.).unit("[0;1]").help("Long-wave extinction coefficient");
    Input(sigma).equals(0.2).help("Scattering coefficient");
    Input(g0).equals(0.1).unit("m/s").help("Ball-Berry model of stomatal resistance (H2O): intercept");
    Input(g1).equals(1.64).unit("m3/mol").help("Ball-Berry model of stomatal resistance (H2O): slope");
    Input(re).equals(200.).unit("m/s").help("Boundary layer resistance (H2O)");
    Input(lai).equals(1.).unit("m2/m2").help("Leaf area index in the cultivated area");
    Input(coverage).equals(0.9).unit("m2/m2").help("Proportion of floor covered by plants");

    Input(Jmax).equals(194).unit("μmol/m2/s").help("Maximum rate of electron transport at 25 oC");
    Input(Vcmax).equals(112).unit("μmol/m2/s").help("Maximum carboxylation rate at 25 oC");
    Input(GammaStar).equals(41).unit("ppm").help("CO2 compensation point");
    Input(Km).equals(600).unit("Pa").help("Michaelis-Menten coefficient for Farquhar model");
    Input(Rd0).equals(0.49).unit("μmol/m2/s").help("Respiration rate at 25 oC");

    Input(alpha).equals(0.24).help("Slope of light response curve");
    Input(theta).equals(0.85).help("Shape of light response curve");
    Input(EaVJ  ).equals(29680).help( "Temperature response of Jmax");
    Input(EdVJ ).equals(200000).help("Temperature response of Jmax");
    Input(delsJ).equals(631.88).help("Temperature response of Jmax");
    Input(EaVC  ).equals(58550).help ("Temperature response of Vcmax");
    Input(EdVC ).equals(200000).help("Temperature response of Vcmax");
    Input(delsC).equals(629.26).help("Temperature response of Vcmax");
    Input(Q10  ).equals(1.92).help("Temperature response of respiration");

    Input(timeStep).imports("calendar[timeStepSecs]");
    Input(indoorsTemperature).imports("indoors[temperature]");
    Input(indoorsRh).imports("indoors[rh]");
    Input(indoorsCo2).imports("indoors[co2]");
    Input(absorbedTop).imports("budget/plant[absorbedTop]");
    Input(absorbedBottom).imports("budget/plant[absorbedBottom]");
    Input(parAbsorbedTop).imports("budget/plant[parAbsorbedTop]");
    Input(parAbsorbedBottom).imports("budget/plant[parAbsorbedBottom]");

    Output(temperature).unit("oC").help("Leaf temperature");
    Output(transpiration).unit("kg/m2 ground/s").help("Transpiration rate");
    Output(Pn).unit("μmol CO2/m2 ground/s").help("Net photosynthetic rate");
    Output(Rd).unit("μmol CO2/m2 ground/s").help("Respiration rate");
    Output(leafAm).unit("μmol CO2/m2 leaf/s").help("Leaf net photosynthetic rate");
    Output(leafAc).unit("μmol CO2/m2 leaf/s").help("Leaf net photosynthetic rate; Rubisco-limited");
    Output(leafAj).unit("μmol CO2/m2 leaf/s").help("Leaf net photosynthetic rate; electron transport-limited");
    Output(leafRd).unit("μmol CO2/m2 leaf/s").help("Leaf respiration rate");
}

void Plant::reset() {
    _rhoh = (1 - sqrt(1-sigma))/(1 + sqrt(1-sigma));
    update();
}

void Plant::update() {
    _svp         = svp(indoorsTemperature);
    _vp          = vpFromRh(indoorsTemperature, indoorsRh);
    _svpSlope    = svpSlope(indoorsTemperature);
    _ri          = ri();
    incidentPar_ = (parAbsorbedTop + parAbsorbedBottom)/lai/swAbsorptivityTopAdj/timeStep;
    updateRadiative();
    updateTemperature();
    updateTranspiration();
    updateLeafPhotosynthesis();
    updateCanopyPhotosynthesis();
}

void Plant::updateRadiative() {
    swReflectivityTopAdj    =
    swReflectivityBottomAdj = reflectivity(k_sw);
    swAbsorptivityTopAdj    =
    swAbsorptivityBottomAdj = absorptivity(k_sw);
    swTransmissivityTopAdj  =
    swTransmissivityBottomAdj = 1. - swReflectivityTopAdj - swAbsorptivityTopAdj;

    lwReflectivityTopAdj    =
    lwReflectivityBottomAdj = reflectivity(k_lw);
    lwAbsorptivityTopAdj    =
    lwAbsorptivityBottomAdj = absorptivity(k_lw);
    lwTransmissivityTopAdj  =
    lwTransmissivityBottomAdj = 1. - lwReflectivityTopAdj - lwAbsorptivityTopAdj;

    checkOutputs();
}

void Plant::updateTemperature() {
    double
        Arad = absorbedTop + absorbedBottom,
        a = (_ri+re)/2./lai/RhoAir/CpAir*Arad - 1./Psychr*(_svp - _vp),
        b = 1. + _svpSlope/Psychr + _ri/re;
    temperature = a/b;
}

void Plant::updateTranspiration() {
    double
        Arad = absorbedTop + absorbedBottom,
        a = _svpSlope/Psychr*Arad + 2.*lai*RhoAir*CpAir/Psychr/re*(_svp - _vp),
        b = LHe*(1. + _svpSlope/Psychr + _ri/re);
    transpiration = a/b;
}

void Plant::updateLeafPhotosynthesis() {
    // Vcmax, Jmax T responses
    VcmaxAdj_ = Vcmax*TVcmax();
    JmaxAdj_  = Jmax *TJmax();

    double
        &GS(_ri),
        &GB(re),
        // Added: Total conductance
        GT = GS*GB/(GS + GB),
        //  Constants
        GCtoGW = 1.57,     // # conversion from conductance to CO2 to H2O
        // Leaf respiration
        &Tleaf(temperature),
        Rd = Rd0*pow(Q10,(Tleaf - 25.)/10.),
        // Electron transport rate
        J = Jfun(),
        VJ = J/4.,
        // Solution when Rubisco activity is limiting
        // Changes: Use total conductance in stead of stomatal conductance
        &Ca(indoorsCo2),
        GC = GT / GCtoGW,
        A = 1./GC,
        B = (Rd - VcmaxAdj_)/GC - Ca - Km,
        C = VcmaxAdj_ * (Ca - GammaStar) - Rd * (Ca + Km),
        Ac = quadM(A,B,C) + Rd,
        // Photosynthesis when electron transport is limiting
        B2 = (Rd - VJ)/GC - Ca - 2*GammaStar,
        C2 = VJ * (Ca - GammaStar) - Rd * (Ca + 2*GammaStar),
        Aj = quadM(A,B2,C2) + Rd,
        // Hyperbolic minimum.
        Am = -quadP(1. - 1e-04, Ac+Aj, Ac*Aj);
        // Net photosynthesis
        Am -= Rd;
    // Done
    leafAm = Am;
    leafAc = Ac;
    leafAj = Aj;
    leafRd = Rd;
}

void Plant::updateCanopyPhotosynthesis() {
    Pn = coverage*leafAm*lai;
    Rd = coverage*leafRd*lai;
}

double Plant::ri() const {
    return 1./(g0 + g1*indoorsRh/100.*Pn/indoorsCo2);
}

double Plant::reflectivity(double k) const {
    return (exp(-k*lai) - exp(k*lai)) / (_rhoh*exp(-k*lai) - exp(k*lai)/_rhoh);
}

double Plant::absorptivity(double k) const {
    return 1. - exp(-k*lai);
}

double Plant::Tadj(double EaV, double EdV, double dels) const {
    const double &Tleaf(temperature),
        V1 = 1. + exp((dels*(25.   + T0)-EdV)/(R*(25.   + T0))),
        V2 = 1. + exp((dels*(Tleaf + T0)-EdV)/(R*(Tleaf + T0))),
        f = V1/V2;
    return exp((Tleaf - 25.)*EaV/(R*(Tleaf + T0)*(25. + T0))) * f;
}

double Plant::TVcmax() const {
    return Tadj(EaVC, EdVC, delsC);
}

double Plant::TJmax() const {
    return Tadj(EaVJ, EdVJ, delsJ);
}

// Non-rectangular hyperbola
double Plant::Jfun() const {
    const double
        &PPFD(incidentPar_),
        &Jmax(JmaxAdj_);
    return
        (alpha*PPFD + Jmax - sqrt(p2(alpha*PPFD + Jmax) - 4*alpha*theta*PPFD*Jmax))/(2*theta);
}

// Larger root
double Plant::quadP(double A, double B, double C) const {
    double D = B*B - 4*A*C;
    if(D < 0)
        ThrowException("Cannot find roots").context(this);

    if (eqZero(A))
        return eqZero(B) ? 0. : -C/B;
    else
        return (-B + sqrt(B*B - 4*A*C)) / (2*A);
}

// Smaller root
double Plant::quadM(double A, double B, double C) const {
    double D = B*B - 4*A*C;
    if (D < 0)
        ThrowException("Cannot find roots").context(this);

    if (eqZero(A))
        return eqZero(B) ? 0. : -C/B;
    else
        return (-B - sqrt(B*B - 4*A*C)) / (2*A);
}

} //namespace
