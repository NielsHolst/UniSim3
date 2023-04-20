/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "budget.h"
#include <base/phys_math.h>
#include <base/publish.h>

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(Budget)

Budget::Budget(QString name, Box *parent)
	: Box(name, parent)
{
    help("computes greenhouse energy and CO2 budgets");
    Input(date).imports("calendar[date]", CA);
    Input(skyIrradiationRate).imports("outdoors[radiation]", CA);
    Input(skyRadiationAbsorbedRate).imports("energyBudget/sky[absorbed]", CA);
    Input(skySwRadiationAbsorbedRate).imports("energyBudget/sky[swAbsorbed]", CA);
    Input(skyLwRadiationAbsorbedRate).imports("energyBudget/sky[lwAbsorbed]", CA);
    Input(ventilationEnergyRate).imports("indoors/temperature[advectiveEnergyFlux]", CA);
    Input(convectionEnergyRate).imports("energyBudget/cover[convectiveInflux]", CA);
    Input(heatSinkEnergyRate).computes("if exists(heatSink[value]) then heatSink[value] else 0");
    Input(groundArea).imports("geometry[groundArea]", CA);
    Input(indoorsAh).imports("indoors/humidity[ah]", CA);
    Input(outdoorsAh).imports("outdoors[ah]", CA);
    Input(soilEnergyRate).imports("energyBudget/floor[conductiveInflux]", CA);
    Input(heatingPowerUsage).imports("actuators/heating[energyFluxTotal]",CA).unit("W/m2");
    Input(growthLightPowerUsage).imports("actuators/growthLights[powerUsage]",CA).unit("W/m2");
    Input(parAbsorbed).imports("energyBudget/crop[parAbsorbed]", CA).unit("umol/s/m2");
    Input(co2Flux).imports("actuators/co2Injection[value]",CA).unit("g/m2/h");
    Input(netGrowthRate).imports("crop/photosynthesis[Pn]",CA).unit("g dry mass/ground m2/h");
    Input(dt).imports("calendar[timeStepSecs]", CA).unit("s");

    Output(lightUseEfficiency).help("Biomass produced per absorbed light ").unit("g/mol absorbed");
    Output(skyIrradiation).help("Accumulated sunlight irradiation").unit("kWh/m2");
    Output(skyRadiationAbsorbed).help("Accumulated radiation lost to the sky").unit("kWh/m2");
    Output(skySwRadiationAbsorbed).help("Accumulated sw radiation lost to the sky").unit("kWh/m2");
    Output(skyLwRadiationAbsorbed).help("Accumulated lw radiation lost to the sky").unit("kWh/m2");
    Output(ventilationEnergy).help("Energy lost by ventilation incl. leakage").unit("kWh/m2");
    Output(convectionEnergy).help("Energy lost by convection").unit("kWh/m2");
//    Output(latentHeatEnergy).help("Energy lost by latent heat in air flux").unit("kWh/m2");
    Output(soilEnergy).help("Energy lost to soil by conduction from floor").unit("kWh/m2");
    Output(heatingEnergy).help("Accumulated energy spent on heating").unit("kWh/m2");
    Output(growthLightEnergy).help("Accumulated energy spent on growth lights").unit("kWh/m2");

    Output(parAbsorbedTotal).help("Accumulated PAR absorbed by plants").unit("kmol/m2");
    Output(co2Total).help("Accumulated CO2 spent").unit("kg/m2");
}

void Budget::initialize() {
}

void Budget::reset() {
   _kiloHour = dt/3600./1000.; // this conversion is for accumulating energy by time step
   _kiloMol = dt*1e-9;
   _prevDate = QDate();
   _hasResetSums = false;
}

void Budget::update() {
    checkNewYear();
    lightUseEfficiency = (netGrowthRate>0. && parAbsorbed>0.) ? netGrowthRate / (parAbsorbed*3600.*1e-6) : 0.;
    // W/m2 = m3/m2 * kg/m3 * J/kg / s;
//    double latentHeatRate = airInflux/groundArea*(outdoorsAh-indoorsAh)*LHe/dt;
    skyIrradiation       += skyIrradiationRate*_kiloHour;
    skyRadiationAbsorbed += skyRadiationAbsorbedRate*_kiloHour;
    skySwRadiationAbsorbed += skySwRadiationAbsorbedRate*_kiloHour;
    skyLwRadiationAbsorbed += skyLwRadiationAbsorbedRate*_kiloHour;
    ventilationEnergy    += ventilationEnergyRate*_kiloHour;
    convectionEnergy     += convectionEnergyRate*_kiloHour;
    heatSinkEnergy       += heatSinkEnergyRate*_kiloHour;
//    latentHeatEnergy     += latentHeatRate*_kiloHour;
    soilEnergy           += soilEnergyRate*_kiloHour;
    heatingEnergy        += heatingPowerUsage*_kiloHour;
    growthLightEnergy   += growthLightPowerUsage*_kiloHour;
    parAbsorbedTotal     += parAbsorbed*_kiloMol;
    co2Total             += co2Flux*_kiloHour;
}

void Budget::checkNewYear()  {
    if (!_hasResetSums && _prevDate.isValid() && date.day()==1 && date.month()==1 && date!=_prevDate) {
        skyIrradiation =
        skyRadiationAbsorbed =
        skySwRadiationAbsorbed =
        skyLwRadiationAbsorbed =
        ventilationEnergy =
        convectionEnergy =
        latentHeatEnergy =
        soilEnergy =
        heatingEnergy =
        growthLightEnergy =
        parAbsorbedTotal =
        co2Total = 0.;
    }
    _prevDate = date;
    _hasResetSums = true;
}

} //namespace

