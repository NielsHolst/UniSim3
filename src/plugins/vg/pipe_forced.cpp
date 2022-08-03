/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include <base/test_num.h>
#include "pipe_forced.h"

#include <iostream>

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(PipeForced)

PipeForced::PipeForced(QString name, Box *parent)
	: Box(name, parent)
{
    help("computes outflow temperature and effect (simplified for IG)");
    Input(Tair).imports("indoors/temperature[value]",CA);
    Input(innerDiameter).equals(43.5).help("Pipe inner diameter").unit("mm");
    Input(waterVolume).equals(7.5).help("Total volume of water in pipes in greenhouse").unit("m3");
    Input(flowRate).equals(20.).help("Water flow rate").unit("m3/h");
    Input(minTemperature).equals(0.).help("Minimum inflow temperature").unit("oC");
    Input(maxTemperature).equals(100.).help("Maximum inflow temperature").unit("oC");

    Input(Tinflow).equals(60.).help("Temperature of inflowing water").unit("oC");
    Input(k).equals(1.66e-4).help("Heat pipe parameter").unit("K/mm/min");
    Input(b).equals(1.23).help("Heat pipe parameter").unit(">1");
    Input(knownToutflow).help("If zero then Toutflow will be calculated").unit("oC");
    Output(transitTime).help("Transit time of water through pipe").unit("min");
    Output(Toutflow).help("Temperature of outflowing water").unit("oC");
    Output(energyFlux).help("Effect of pipe per greenhouse area").unit("W/m2");
}

void PipeForced::reset() {
    Toutflow = (knownToutflow > 0.) ? knownToutflow : Tinflow;
    updateEffect();
}

void PipeForced::update() {
    updateTransitTime();
    updateToutflow();
    updateEffect();
//    std::cout
//        << "\nInputs\n"
//        << Tair << "\n"
//        << innerDiameter << "\n"
//        << waterVolume << "\n"
//        << flowRate << "\n"
//        << Tinflow << "\n"
//        << k << "\n"
//        << b << "\n"
//        << knownToutflow << "\n"
//        << "\nOutputs\n"
//        << transitTime << "\n"
//        << Toutflow << "\n"
//        << effect << "\n";
}

void PipeForced::updateTransitTime() {
    transitTime = (flowRate > 0.) ? waterVolume/flowRate*60. : 0.;
}

void PipeForced::updateToutflow() {
    if (knownToutflow > 0.)
        Toutflow = knownToutflow;
    else {
        const double &d(innerDiameter),
                     &Dt(transitTime),
                     &Tin(Tinflow);
        Toutflow = (Tin <= Tair) ?
                    Tinflow :
                    Tair + pow((k*d*(b-1)*Dt + pow(Tin - Tair, 1-b)), 1/(1-b));
    }
}

void PipeForced::updateEffect() {
    energyFlux = (Tinflow-Toutflow)*flowRate*CpWater/3600;
}


}

