/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
    /* Copyright 2005-2019 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/phys_math.h>
#include <base/publish.h>
#include "leaf_temperature.h"

using namespace base;
using namespace phys_math;

namespace vg {

PUBLISH(LeafTemperature)

LeafTemperature::LeafTemperature(QString name, Box *parent)
	: Box(name, parent)
{
    help("models leaf temperature");
    Input(indoorsTemperature).imports("indoors/temperature[value]",CA).unit("oC");
    Input(indoorsRh).imports("indoors/humidity[rh]",CA).unit("[0;100]");
    Input(rsH2O).computes("mean(../photosynthesis/*/stomatalResistance[rH2O])").unit("s/m2");
    Input(rbH2O).imports("../boundaryLayerResistance[rH2O]",CA).unit("s/m2");
    Input(radiationAbsorbed).imports("energyBudget/crop[absorbed]",CA).unit("W/m2");
    Input(lai).imports("/crop[lai]", CA);
    Output(value).help("Leaf temperature").unit("oC");
}

void LeafTemperature::reset() {
    value = indoorsTemperature;
}

void LeafTemperature::update() {
    // Stanghellini (1987, eq. 3.5-3.6), ignoring J (thermal storage)
    double Rn = radiationAbsorbed/(2*lai),
           s = svpSlope(indoorsTemperature),
           psatu = svp(indoorsTemperature),
           pgh = vpFromRh(indoorsTemperature, indoorsRh),
           dividend = (rsH2O+rbH2O)*Rn/(2*lai*RhoAir*CpAir) - (psatu-pgh)/Psychr,
           divisor  = 1. + s/Psychr+ rsH2O/rbH2O;
    value = dividend/divisor + indoorsTemperature;
    if (fabs(value) > 100.) {
        QString msg("s = %1\nrsH2O = %2\nrbH2O = %3\nRn = %4\nTin = %5");
        ThrowException("Leaf temperature out of bounds").value(value).context(this).
                hint(msg.arg(s).arg(rsH2O).arg(rbH2O).arg(Rn).arg(indoorsTemperature));
    }

    /* Thermal storage was neglible, max 1-2 W/m2
    double dt = 300,
           sla = 30,
           C = 300;
    dT = value-prevValue;
    thermalStorage = dT*lai/sla*C/dt;
    */
}


} //namespace

