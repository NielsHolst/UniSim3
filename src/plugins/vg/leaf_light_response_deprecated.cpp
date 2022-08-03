/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "leaf_light_response_deprecated.h"

using namespace phys_math;
using namespace base;

namespace vg {
	
PUBLISH(LeafLightResponseDeprecated)

LeafLightResponseDeprecated::LeafLightResponseDeprecated(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes the light response curve");
    Input(rsCO2).imports("../../rs[rsCo2]",CA).unit("s/m");
    Input(rbCO2).imports("../../rb[rbCo2]",CA).unit("s/m");
    Input(Tleaf).imports("../../temperature[value]",CA).unit("oC");
    Input(co2Air).imports("indoors/co2[value]",CA).unit("ppm");
    Input(VCmax).imports("./processes[VCmax]",CA);
    Input(Jmax).imports("./processes[Jmax]",CA);
    Input(gamma).imports("./processes[gamma]",CA);
    Input(KM).imports("./processes[KM]",CA);
    Input(theta).equals(0.7).help("Shape parameter for the light response curve").unit("-");
    Input(frParAbs).equals(0.3).help("Fraction of PAR absorbed [0;1]").unit("[0;1]");
    Output(lue).help("Light use efficiency").unit("mg CO2/umol photons");
    Output(Pnmax).help("Net assimilation rate").unit("mg CO2/leaf m2/s");
    Output(Pgmax).help("Gross assimilation rate").unit("mg CO2/leaf m2/s");
    Output(Rd).help("Dark respiration rate").unit("mg CO2/leaf m2/s");
    Output(rtCO2).help("Total CO2 resistance against").unit("s/m");
}

void LeafLightResponseDeprecated::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box*>("./processes"))
        builder.
        box("LeafLightResponseProcessesDeprecated").name("processes").
        endbox();
}

void LeafLightResponseDeprecated::reset() {
    lue = 0.0155;
    Pnmax = 0.8;
    Rd = 0.015;
    Pgmax = Pnmax + Rd;
}

void LeafLightResponseDeprecated::update() {
    // If CO2a_ppm < gamma: no photosynthesis
    if (co2Air < gamma) {
        lue = Pnmax = 0.;
    }
    // Reduction of light use efficiency by photorespiration, affected by CO2 concentration
    else {
        lue = potentialLightUseEfficiency()*(co2Air-gamma)/(co2Air+2*gamma);
        Pnmax = maxNetAssimilation();
    }
    Rd = darkRespirationRate();
    Pgmax = Pnmax + Rd;    // maximum gross assimilation [mg CO2/m2 leaf/s]
    if (Rd > 1e10 || Pgmax > 1e10)
        ThrowException("Infinite light response variables").value(Rd).value2(Pgmax).context(this);
}

double LeafLightResponseDeprecated::potentialLightUseEfficiency() {
    double alpha = (1-frParAbs)/2;  // Electron yield of absorbed photons at lowlight intensity:
                                    // 2 electrons per absorbed photon [umol e-/umol photons]
    return MCo2/4*alpha;          // [mg CO2/umol photons]
}

double LeafLightResponseDeprecated::darkRespirationRate() {
    const double ED = 66405,                 // RD dark respiration rate
                 RD25 = 1.1;                 // [F80, p.89] dark respiration rate [umol CO2/m2/s]
    double Q10 = exp(13.6e-6*ED);            // (2.0) Q10 RD
    return RD25*MCo2*pow(Q10,(Tleaf-25)/10); // dark respiration rate [umol CO2/m2/s * mg CO2/umol CO2 = mg CO2/m2/s]
}

double LeafLightResponseDeprecated::maxNetAssimilation() {
    double RcCo2 = KM/VCmax/MCo2,     // carboxylation resistance [s/m]
           RCo2 = rsCO2+rbCO2,
           TleafK = Tleaf + T0,
           rhoCo2T = RhoCo2*T0/TleafK;      // [gaslaw] density CO2 at Tleaf [kg/m3], [mg/ml]

    rtCO2 = RCo2+RcCo2;                  // [G94, p.79] total resistance to CO2 diffusion [s/m]
    double PNC = (co2Air-gamma)*rhoCo2T/rtCO2;
    return (Jmax + PNC - sqrt(sqr(Jmax+PNC) - 4*theta*Jmax*PNC))/(2*theta);
}

} //namespace

