/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "indoors_co2.h"

using namespace base;

namespace vg {

PUBLISH(IndoorsCo2)

IndoorsCo2::IndoorsCo2(QString name, Box *parent)
	: Box(name, parent)
{
    help("models indoors CO2 concentration");
    Input(outdoorsCo2).imports("outdoors[co2]",CA).unit("ppm");
    Input(airFlux).imports("indoors/ventilation[flux]",CA).unit("m3/m3/h");
    Input(injectionRate).imports("actuators/co2Injection[value]",CA).unit("g/m2/h");
    Input(assimilation).imports("crop/photosynthesis[Pg]",CA).unit("[g CO2/m2/h]");
    Input(averageHeight).imports("geometry[averageHeight]",CA).unit("m");
    Input(timeStep).imports("calendar[timeStepSecs]").unit("s");
    Output(value).help("Indoors CO2 concentration [ppm]").unit("ppm");
}

void IndoorsCo2::reset() {
    value = outdoorsCo2;
}

namespace {
    double integrate(double a,  // injection rate - assimilation rate
                     double b,  // ventilation rate
                     double c,  // outdoors CO2
                     double y0, // initial indoors CO2
                     double dt) // time step
    {
        return (b==0.) ?
            y0 + a*dt :
            exp(-b*dt)*( (exp(b*dt)-1)*(a/b+c)+y0 );
    }
}

void IndoorsCo2::update() {
    value = integrate(
                (injectionRate-assimilation)/averageHeight/1.83e-3,   // ppm/h = g/m2/h / m / (g/m3/ppm)
                airFlux,
                outdoorsCo2,
                value,
                timeStep/3600.
                );
}

} //namespace

