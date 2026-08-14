/* Copyright 2005-2021 by Niels Holst, Aarhus University [niels.holst at agro.au.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "growth.h"

using namespace base;
using namespace phys_math;
using namespace std;

namespace coffee {

PUBLISH(Growth)

Growth::Growth(QString name, QObject *parent)
    : Box(name, parent) {
    help("models coffee growth");
    Input(temperature).imports("weather[Tavg]");
    Input(parAverage).imports("light[parShadeAvg]");
    Input(parIntercepted).imports("light[parInterceptedCoffee]");
    Input(parAverage30Days).imports("weather[AvgPar30]");
    Input(growthStage).imports("phenology[growthStage]");
    Input(transpirationRatio).imports("coffee/waterFlux[transpirationRatio]");
    Input(airCo2).equals(780.).unit("ppm").help("Concentration of CO2 in the air");
    Input(k).imports("coffee[k]");
    Input(convEfficiency).equals(0.74).unit("kg C/kg C").help("Conversion efficiency");
    Input(sinkLeaf).equals(1.2).unit("-").help("Sink strength of leaves");
    Input(sinkWoody).equals(2.1).unit("-").help("Sink strength of woody tissue (stem+branches)");
    Input(sinkRoot).equals(2.5).unit("-").help("Sink strength of roots");
    Input(sinkBerryMax).equals(6.4).unit("-").help("Max. sink strength of berries");
    Input(sinkBerryCoef).equals(0.55).unit("-").help("Sink strength of berries dependence on light");

    Output(sinkBerry).unit("-").help("Sink strength of berries");
    Output(gTotalC).help("Potential carbon growth rate");
    Output(gLeafC).help("Growth rate of leaf carbon");
    Output(gWoodyC).help("Growth rate of woody carbon");
    Output(gRootC).help("Growth rate of root carbon");
    Output(gBerryC).help("Growth rate of berry carbon");
}

void Growth::update() {
    const double
        // Michaelis-Menten constants for CO2 and O2 at 250C
        KMC25 = 460,    // (ppm CO2)
        KMO25 = 33,     // (% O2)    --> is this constant not too low (by a factor of 10)?
        // Other constants related to the Farquhar-type photosynthesis model
        O2 = 21,        // (% O2)    --> is this constant not too low (by a factor of 10)?
        EAVCMX = 68000, // (J mol-1)  energy of activation for Vcmx
        EAKMO  = 1400,  // (J mol-1)  energy of activation for KMO
        R = 8.314;      // Gas constant
    // Michaelis-Menten constants for CO2 and O2, respectively
    double
        KMC = KMC25 * exp((1/(25+T0)-1/(temperature+T0)) * EAVCMX/R),   // (ppm CO2)
        KMO = KMO25 * exp((1/(25+T0)-1/(temperature+T0)) * EAKMO/R);   // % O2
    // CO2 compensation point in the absence of dark respiration
    const double KOKC = 0.21;
    // CO2 compensation point
    double GAMMAX = 0.5 * KOKC * KMC * O2 / KMO;    // (ppm CO2)
    // Arrhenius function for the effect of temperature on carboxylation
    double VCT = exp((1/(25+T0)-1/(temperature+T0))*EAVCMX/R);
    const double
        RUBISC = 0.54,     // 0.5-1.5 (g m-2)
        KC25 = 138;        // [g CO2 g-1 Rubisco d-1]
    // Probably the max rate of binding of CO2 (max velocity of carboxylation in upper leaves)
    double VCMAX = RUBISC * KC25 * VCT; // (g CO2 m-2 leaf d-1)
    // See van Oijen et al. (2004)
    // Leaf internal CO2 concentration
    double CO2I = 0.7 * airCo2;   // (ppm CO2) CO2 atmosphere = actual value of CO2
    double PMAX = VCMAX * (CO2I-GAMMAX) / (CO2I +KMC * (1+O2/KMO));    // (g CO2 m-2 leaf d-2)
    // Conversion factor in umol/J  --> conversion factor for light energy to photon density
    const double JMUMOL = 4.56;
    // 44 = molecular weight of CO2; 2.1, 4.5, and 10.5 are constants related to the stoichiometry
    // for electron transport and CO2 fixation
    double EFF = 44 * JMUMOL/2.1 * (CO2I - GAMMAX) / (4.5*CO2I + 10.5 * GAMMAX); // (g CO2 MJ-1 PAR)
    double LUECO2 = EFF * PMAX / (EFF * k * parAverage + PMAX);     // (g CO2 MJ-1 PAR)

    /*  Carbon assimilation
    The source strength (potential amount of C which can be assimilated by photosynthesis) is computed from the PAR intercepted by the coffee,
    the light-use efficiency (computed from the atmospheric CO2, temperature, the photoperiod-average of PAR, the light extinction coefficient,
    and leaf RUBISCO content). Photosynthesis is also modulated by the water status in soil (as expressed in the ratio of actual to potential
    transpiration rate, the so-called 'transpiration realisation factor') and is assumed to be zero at days of pruning (the "NOPRUN" variable=0).
    Finally, the assimilation carbon is modulated by constant growth efficiency */

    // Net carbon growth; 12/44 = carbon content of CO2
    gTotalC = convEfficiency*LUECO2*0.001*12./44.*parIntercepted*transpirationRatio;

    // Adjusted sink strengths
    sinkBerry = sinkBerryMax*(1. - exp(-sinkBerryCoef*parAverage30Days));
    double sinkRootAdj  = sinkRoot*(2-transpirationRatio),
           sinkBerryAdj = sinkBerry*min(2*growthStage, 1.);

    // # Total sink strength
    double sinkTotal = sinkLeaf + sinkWoody + sinkRootAdj + sinkBerryAdj;

    // Divide carbon among organs according to their relative sink strengths
    gLeafC  = gTotalC*sinkLeaf   /sinkTotal,
    gWoodyC = gTotalC*sinkWoody   /sinkTotal,
    gRootC  = gTotalC*sinkRootAdj/sinkTotal,
    gBerryC = gTotalC*sinkBerryAdj/sinkTotal;
}

} //namespace

