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
#include "pipe.h"

#include <base/dialog.h>

using namespace base;
using namespace phys_math;
using namespace std;

namespace vg {
	
PUBLISH(Pipe)

Pipe::Pipe(QString name, Box *parent)
	: Box(name, parent)
{
    help("computes outflow temperature and effect");
    Input(material).equals("iron").help("Material type").unit("text id");
    Input(density).equals(1.).help("Pipe length per greenhouse area").unit("m/m2");
    Input(diameter).equals(51.).help("Pipe inner diameter").unit("mm");
    Input(flowRate).equals(5.).help("Water flow rate").unit("L/s");
    Input(minTemperature).equals(20.).help("Minimum inflow temperature").unit("oC");
    Input(maxTemperature).equals(80.).help("Maximum inflow temperature").unit("oC");
    Input(k).equals(1.5e-4).help("Heat pipe effect coefficient").unit("K/mm/s");
    Input(b).equals(1.4).help("Heat pipe effect coefficient").unit("!=1");

    Input(inflowTemperature).imports("actuators/heating[temperature]",CA).unit("oC");
    Input(indoorsTemperature).imports("indoors/temperature[value]",CA).unit("oC");
    Input(groundArea).imports("geometry[groundArea]",CA).unit("m2");
    Input(numSpans).imports("geometry[numSpans]",CA);

    Output(emissivity).help("Emissivity of pipe").unit("[0;1]");
    Output(length).help("Total pipe length").unit("m");
    Output(volume).help("Total pipe water volume").unit("m3");
    Output(lengthPerSpan).help("Pipe length in each greenhouse span").unit("m");
    Output(volumePerSpan).help("Pipe water volume in each greenhouse span").unit("m3");
    Output(transitTime).help("Average transit time of water").unit("s");
    Output(outflowTemperature).help("Water temperature at exit").unit("oC");
    Output(temperatureDrop).help("Drop in water temperature from entry to exit").unit("oC");
    Output(energyFlux).help("Energy flux").unit("W/m2");

    materialInputs["iron"]         = MaterialInputs{0.8};
    materialInputs["polyethylene"] = MaterialInputs{0.1};
    materialInputs["copper"]       = MaterialInputs{0.2};
    materialInputs["aluminium"]    = MaterialInputs{0.2};
    parseMaterial();
}

void Pipe::reset() {
    parseMaterial();
    length = density*groundArea; // m = m/m2 * m2
    volume = PI/4.*sqr(diameter/1000.)*length;
    lengthPerSpan = length/numSpans;
    volumePerSpan = volume/numSpans;
    _k = k*diameter; // K/s
    update();
}

void Pipe::update() {
    if (TestNum::eq(flowRate, -999)) flowRate = 5.;
    transitTime = 1000.*volumePerSpan/flowRate;
    inflowTemperature = minmax(indoorsTemperature, inflowTemperature, maxTemperature);
    double x = _k*(b-1.)*transitTime + pow(inflowTemperature-indoorsTemperature, 1-b);
    temperatureDrop = inflowTemperature - indoorsTemperature - pow(x, 1./(1.-b));
    outflowTemperature = inflowTemperature - temperatureDrop;
    double E = CpWater*volumePerSpan*temperatureDrop*1000.;
    energyFlux = numSpans*E/groundArea/transitTime;
}

void Pipe::parseMaterial() {
    QString s = material.toLower();
    if (!materialInputs.contains(s))
        ThrowException("Unknown pipe material").value(material).
                hint(QStringList(materialInputs.keys()).join(",")).context(this);
    emissivity = materialInputs.value(s).emissivity;
}

}

