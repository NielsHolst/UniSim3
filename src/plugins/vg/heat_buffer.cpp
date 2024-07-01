/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include <base/phys_math.h>
#include <base/test_num.h>
#include "heat_buffer.h"

using namespace base;
using namespace phys_math;
using namespace TestNum;

namespace vg {

PUBLISH(HeatBuffer)

HeatBuffer::HeatBuffer(QString name, Box *parent)
    : Box(name, parent)
{
    help("models a heat buffer");
    Input(volume).equals(10).help("Buffer volume").unit("m3");
    Input(maxTemperature).equals(95.).help("Max. temperature of water in buffer").unit("oC");
    Input(U).equals(10.).help("U-value relative to surroundings").unit("W/K");
    Input(uptakeEfficiency).equals(0.8).unit("[0;1]").help("Efficiency of heat uptake into the buffer");
    Input(extractionEfficiency).equals(0.8).unit("[0;1]").help("Efficiency of heat extraction from the buffer");

    Input(inputResource).help("Ressource power available, e.g. from a heat pump").unit("W/m2");
    Input(outputDemand).help("Demanded power for heating").unit("W/m2");
    Input(timeStep).imports("calendar[timeStepSecs]");

    Input(groundArea).imports("geometry[groundArea]");
    Input(externalTemperature).imports("outdoors[temperature]").help("Temperature of surroundings");

    Output(inputDemand).help("Power demanded to reach `maxTemperature` during `timeStep`").unit("W/m2");
    Output(inputSupply).help("Power supplied by `inputResource` in response to `inputDemand`").unit("W/m2");
    Output(inputOverflow).help("Power supplied by `inputResource` but left unused, because the buffer was full").unit("W/m2");
    Output(outputResource).help("Power available to fulfill `outputDemand`").unit("W/m2");
    Output(outputSupply).help("Power supplied by `outputResource` in response to `outputDemand`").unit("W/m2");
    Output(heatLoss).help("Power lost to surroundings").unit("W");
    Output(temperature).help("Temperature of water in buffer").unit("oC");
}

void HeatBuffer::reset() {
    temperature = 20.;
    if (uptakeEfficiency <= 0. || uptakeEfficiency > 1.)
        ThrowException("uptakeEfficiency must be >0 and <=1");
    if (extractionEfficiency <= 0. || extractionEfficiency > 1.)
        ThrowException("extractionEfficiency must be >0 and <=1");
    update();
}

void HeatBuffer::update() {
    // Convert W to delta T
    const double CpSpec = timeStep/CpWater/volume*1e-3; // s * kg * K / J / m3 * 1e-3 m3 / kg = K/W
    // Decrease (rarely increase) buffer temperature by heat loss
    heatLoss = U*(temperature - externalTemperature);
    temperature -= heatLoss*CpSpec;
    // Increase buffer temperature by power input
    inputDemand = (temperature < maxTemperature) ? (maxTemperature - temperature)/CpSpec : 0.;
    double potentialInput = uptakeEfficiency*inputResource*groundArea;
    inputSupply = TestNum::eqZero(inputDemand) ? 0. : inputDemand*(1. - exp(-potentialInput/inputDemand));
    temperature += inputSupply*CpSpec;
    // Compute any unused input
    inputOverflow = potentialInput - inputSupply;
    snapToZero(inputOverflow, 1e-3);
    // Decrease buffer temperature by power output
    const double minTemperature = std::max(externalTemperature, 0.),
                 grossDemand = outputDemand/extractionEfficiency;
    outputResource = std::max(temperature - minTemperature, 0.)/CpSpec/groundArea; // W/m2
    const double grossSupply = TestNum::eqZero(grossDemand) ? 0. : grossDemand*(1. - exp(-outputResource/grossDemand));
    outputSupply = extractionEfficiency*grossSupply;
    temperature -= grossSupply*groundArea*CpSpec;
    // Round off
    temperature = minmax(minTemperature, temperature, maxTemperature);
    // Unit conversions
    inputDemand /= groundArea;
    inputSupply /= groundArea;
    inputOverflow /= groundArea;
}

} //namespace

