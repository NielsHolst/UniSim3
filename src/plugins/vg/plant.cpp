/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    : LayerAsOutput(name, parent)
{
    Input(k_sw).equals(0.7).unit("[0;1]").help("Short-wave extinction coefficient");
    Input(k_lw).equals(1.).unit("[0;1]").help("Long-wave extinction coefficient");
    Input(sigma).equals(0.2).help("Scattering coefficient");
    Input(g0).equals(0.1).unit("m/s").help("Ball-Berry model of stomatal resistance (H2O): intercept");
    Input(g1).equals(1.64).unit("m3/mol").help("Ball-Berry model of stomatal resistance (H2O): slope");
    Input(re).equals(200.).unit("m/s").help("Boundary layer resistance (H2O)");
    Input(lai).equals(1.).unit("m2/m2").help("Leaf area index in the cultivated area");
    Input(coverage).equals(0.9).unit("m2/m2").help("Proportion of floor covered by plants");

    Input(Jmax).equals(194).unit("&micro;mol/m2/s").help("Maximum rate of electron transport at 25 oC");
    Input(Vcmax).equals(112).unit("&micro;mol/m2/s").help("Maximum carboxylation rate at 25 oC");
    Input(GammaStar).equals(41).unit("ppm").help("CO2 compensation point");
    Input(Km).equals(600).unit("Pa").help("Michaelis-Menten coefficient for Farquhar model");
    Input(Rd0).equals(0.49).unit("&micro;mol/m2/s").help("Respiration rate at 25 oC");

    Input(alpha).equals(0.24).help("Slope of light response curve");
    Input(theta).equals(0.85).help("Shape of light response curve");
    Input(EaVJ  ).equals(29680).help( "Temperature response of Jmax");
    Input(EdVJ ).equals(200000).help("Temperature response of Jmax");
    Input(delsJ).equals(631.88).help("Temperature response of Jmax");
    Input(EaVC  ).equals(58550).help ("Temperature response of Vcmax");
    Input(EdVC ).equals(200000).help("Temperature response of Vcmax");
    Input(delsC).equals(629.26).help("Temperature response of Vcmax");
    Input(Q10  ).equals(1.92).help("Temperature response of respiration");

    Input(indoorsTemperature).imports("gh/budget/indoors[temperature]");
    Input(indoorsRh).imports("gh/budget/indoors[rh]");
    Input(indoorsCo2).imports("gh/budget/indoors[co2]");
    Input(timeStep).imports("calendar[timeStepSecs]");

    Output(temperature).unit("oC").help("Leaf temperature");
    Output(transpiration).unit("kg/m2 ground/s").help("Transpiration rate");
    Output(incidentPar).unit("&micro;mol PAR/m2 ground/s").help("PAR hitting the canopy");
    Output(Pn).unit("&micro;mol CO2/m2 ground/s").help("Net photosynthetic rate");
    Output(Pg).unit("&micro;mol CO2/m2 ground/s").help("Gross photosynthetic rate");
    Output(Rd).unit("&micro;mol CO2/m2 ground/s").help("Respiration rate");
    Output(growthRate).unit("g carbohydrate/m2 ground").help("Carbohydrate biomass produced in this time step");
    Output(leafAm).unit("&micro;mol CO2/m2 leaf/s").help("Leaf net photosynthetic rate");
    Output(leafAc).unit("&micro;mol CO2/m2 leaf/s").help("Leaf net photosynthetic rate; Rubisco-limited");
    Output(leafAj).unit("&micro;mol CO2/m2 leaf/s").help("Leaf net photosynthetic rate; electron transport-limited");
    Output(leafRd).unit("&micro;mol CO2/m2 leaf/s").help("Leaf respiration rate");
    Output(leafGrowthRate).unit("g carbohydrate/m2 leaf").help("Carbohydrate biomass produced in this time step");
    Output(lue).unit("g carbohydrate/micromole PAR absorbed").help("Light use efficiency");
}

void Plant::reset() {
    temperature = 20.;
    rhoh_ = (1 - sqrt(1-sigma))/(1 + sqrt(1-sigma));
    updateRadiative();
}

void Plant::update() {
    updateRadiative();
}

double Plant::calcGrowthRate(double temperature, double co2) {
    indoorsTemperature = temperature;
    indoorsCo2 = co2;
    update();
    return growthRate;
}

void Plant::saveState() {
    state.indoorsTemperature  = indoorsTemperature;
    state.indoorsCo2 = indoorsCo2;
//    state.netRadiation = netRadiation_;
//    state.parAbsorbed = incidentPar*swAbsorptivityTop;
}

void Plant::restoreState() {
    indoorsTemperature  = state.indoorsTemperature;
    indoorsCo2 = state.indoorsCo2;
//    updateByRadiation(state.netRadiation, state.parAbsorbed);
    update();
}

void Plant::updateByRadiation(double netRadiation, double parAbsorbed) {
    netRadiation_ = netRadiation;
    incidentPar  = parAbsorbed/swAbsorptivityTop;
    svp_          = svp(indoorsTemperature);
    vp_           = vpFromRh(indoorsTemperature, indoorsRh);
    svpSlope_     = svpSlope(indoorsTemperature);
    ri_           = ri();
    updateTemperature();
    updateTranspiration();
    updateLeafPhotosynthesis();
    updateCanopyPhotosynthesis();
    lue = std::max((parAbsorbed > 0.) ? leafAm*co2ToBiomass/parAbsorbed : 0., 0.);
}

void Plant::updateRadiative() {
    // Short-waved
    swReflectivityTop    =
    swReflectivityBottom = reflectivity(k_sw);
    swAbsorptivityTop    =
    swAbsorptivityBottom = absorptivity(k_sw);

    if (swReflectivityTop + swAbsorptivityTop > 1.) {
        swReflectivityTop      =
        swReflectivityBottom   = 1. - swAbsorptivityBottom;
        swTransmissivityTop    =
        swTransmissivityBottom = 0.;
    }
    else {
        swTransmissivityTop  =
        swTransmissivityBottom = 1. - swReflectivityTop - swAbsorptivityTop;
    }
    // Long-waved
    lwReflectivityTop    =
    lwReflectivityBottom = reflectivity(k_lw);
    lwAbsorptivityTop    =
    lwAbsorptivityBottom = absorptivity(k_lw);

    if (lwReflectivityTop + lwAbsorptivityTop > 1.) {
        lwReflectivityTop      =
        lwReflectivityBottom   = 1. - lwAbsorptivityBottom;
        lwTransmissivityTop    =
        lwTransmissivityBottom = 0.;
    }
    else {
        lwTransmissivityTop  =
        lwTransmissivityBottom = 1. - lwReflectivityTop - lwAbsorptivityTop;
    }

    checkParameters();
}

void Plant::updateTemperature() {
    double
        a = (ri_+re)/2./lai/RhoAir/CpAir*netRadiation_ - 1./Psychr*(svp_ - vp_),
        b = 1. + svpSlope_/Psychr + ri_/re;
    temperature = indoorsTemperature + a/b;
}

void Plant::updateTranspiration() {
    if (leZero(netRadiation_))
        transpiration =  0.;
    else {
        double
            a = svpSlope_/Psychr*netRadiation_ + 2.*lai*RhoAir*CpAir/Psychr/re*(svp_ - vp_),
            b = LHe*(1. + svpSlope_/Psychr + ri_/re);
        transpiration = a/b;
    }
}

void Plant::updateLeafPhotosynthesis() {
    // Vcmax, Jmax T responses
    VcmaxAdj_ = Vcmax*TVcmax();
    JmaxAdj_  = Jmax *TJmax();

    double
        &GS(ri_),
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
    leafGrowthRate = leafAm*co2ToBiomass*timeStep*1e-6; // convert from micro gram to gram
}

void Plant::updateCanopyPhotosynthesis() {
    Pn = coverage*leafAm*lai;
    Rd = coverage*leafRd*lai;
    Pg = Pn + Rd;
    growthRate = Pn*co2ToBiomass*timeStep*1e-6; // convert from micro gram to gram;
}

double Plant::ri() const {
    return 1./(g0 + g1*indoorsRh/100.*Pn/indoorsCo2);
}

double Plant::reflectivity(double k) const {
    return (exp(-k*lai) - exp(k*lai)) / (rhoh_*exp(-k*lai) - exp(k*lai)/rhoh_);
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
        &PPFD(incidentPar/lai),
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

