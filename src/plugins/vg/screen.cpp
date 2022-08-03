/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/test_num.h>
#include "screen.h"

using namespace base;
using namespace phys_math;

namespace vg {

Screen::Screen(QString name, Box *parent)
    : HeatTransferLayerParameters(name, parent)
{
    help("models the impact on light and air fluxes of a screen");
    Input(haze).equals(1.).help("Proportion of direct light transmitted as diffuse light (not used)").unit("[0;1]");
    Input(transmissivityAir).equals(0.37).help("Air transmissivity when fully drawn").unit("[0;1]");
    Input(state).equals(0.).help("Proportion drawn (0=fully withdrawn; 1=fully drawn").unit("[0;1]");
    Input(ventilation).imports("indoors/ventilation[flux]");
    Input(Uair).imports("shelter[Uair]");
    Input(UstateExponent).imports("shelter[screenUstateExponent]");
    Input(UventilationSlope).imports("shelter[screenUventilationSlope]");
    port("Utop")->equals(1.247);
    port("Ubottom")->equals(1.247);
    port("heatCapacity")->equals(2280.);
    /* density of polyester = 1.22 g/cm3
     * 1mm * 1m2 = 0.1cm * 100^2 cm2 = 1000 cm3 = 1220 g/m2
     * heat capacity of polyester = 1.87 J/g/K = 1.87*1220 J/m2/K = 2280 J/m2/K
    */
    Output(maxArea).help("Max. area covered by screen").unit("m");
    Output(swReflectivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(swReflectivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(swTransmissivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(swTransmissivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwReflectivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwReflectivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwTransmissivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwTransmissivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(swAbsorptivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(swAbsorptivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwAbsorptivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(lwAbsorptivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(emissivityTopNet).help("Parameter corrected for state").unit("[0;1]");
    Output(emissivityBottomNet).help("Parameter corrected for state").unit("[0;1]");
    Output(UtopNet).help("Parameter corrected for state").unit("W/K/m2 screen");
    Output(UbottomNet).help("Parameter corrected for state").unit("W/K/m2 screen");
}

void Screen::reset() {
    updateByState(0);
    maxArea = computeMaxArea();
}

void Screen::update() {
    updateByState(state);
}

#define ADJUST_BY_STATE(x)    x##Net = state*x
#define ADJUST_BY_STATE_TR(x) x##Net = state*x + 1 - state

void Screen::updateByState(double state) {
    if (state<0. || state>1.)
        ThrowException("Screen state out of [0;1] bounds").value(state).context(this);
    // Prevent numeric precision problems
    if (state < 1e-3)
        state = 0.;
    // Adjust screen area by state
    area = maxArea*state;
    updateAbsorptivities();
    ADJUST_BY_STATE(swReflectivityTop);
    ADJUST_BY_STATE(swReflectivityBottom);
    ADJUST_BY_STATE_TR(swTransmissivityTop);
    ADJUST_BY_STATE_TR(swTransmissivityBottom);
    ADJUST_BY_STATE(lwReflectivityTop);
    ADJUST_BY_STATE(lwReflectivityBottom);
    ADJUST_BY_STATE_TR(lwTransmissivityTop);
    ADJUST_BY_STATE_TR(lwTransmissivityBottom);
    ADJUST_BY_STATE(swAbsorptivityTop);
    ADJUST_BY_STATE(swAbsorptivityBottom);
    ADJUST_BY_STATE(lwAbsorptivityTop);
    ADJUST_BY_STATE(lwAbsorptivityBottom);
    updateU();
}

void Screen::updateU() {
    double adj = exp(-UventilationSlope*ventilation),
           Rair = 1./Uair,
           Rtop = adj*(1./Utop + Rair*pow(state, UstateExponent)),
           Rbottom = adj/Ubottom;
    // R-values are relative to screen m2
    // Convert to relative to shelter face m2
    Rtop *= state;
    Rbottom *= state;
    UtopNet = 1./Rtop;
    UbottomNet = 1./Rbottom;
}

} //namespace

