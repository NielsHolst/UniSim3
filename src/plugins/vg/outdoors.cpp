/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "outdoors.h"
#include <base/box_builder.h>
#include <base/environment.h>
#include <base/phys_math.h>
#include <base/publish.h>

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(Outdoors)

Outdoors::Outdoors(QString name, Box *parent)
	: Box(name, parent)
{
    help("delivers readings of outdoors weather");
    QString Tsky = "if exists(./records[Tsky]) then ./records[Tsky] else skyTemperatureEstimate[temperature]";

    Input(co2).equals(400.).help("Outdoors CO2 concentration").unit("ppm");
    Input(temperature).imports("./records[Tair]").help("Outdoors ambient temperature").unit("oC");
    Input(rh).imports("./records[Rhair]").help("Outdoors ambient relative humidity").unit("[0;100]");
    Input(radiation).imports("./records[GlobRad]").help("Total sunlight irradiation").unit("W/m2");
    Input(propPar).equals(0.45).help("Proportion of PAR in radiation").unit("[0;1]");
    Input(propUv).equals(0.07).help("Proportion of UV in radiation").unit("[0;1]");
    Input(windSpeed).imports("./records[Windspeed]").help("Outdoors wind speed").unit("m/s");
    Input(skyTemperature).computes(Tsky).help("Sky temperature").unit("oC");
    Output(par).help("Sunlight PAR").unit("mymole PAR/m2/s");
    Output(ah).help("Absolute humidity").unit("kg/m3");
    Output(sh).help("Specific humidity").unit("kg/kg");
    Output(soilTemperature).imports("./soilTemperature[value]").unit("oC");
}

void Outdoors::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box*>("./records") && environment().option("dontAutoCreateRecords").isNull())
        builder.
        box("Records").name("records").
            port("fileName").equals("input/sel_dk.txt").
        endbox();
    if (!findMaybeOne<Box*>("./skyTemperatureEstimate"))
        builder.
        box("SkyTemperature").name("skyTemperatureEstimate").
        endbox();
    if (!findMaybeOne<Box*>("./soilTemperature"))
        builder.
        box("Accumulator").name("soilTemperature").
            port("initial").imports("./initial[value]").
            port("change").imports("./controller[controlVariable]").
            box("Hump").name("initial").
                port("x").imports("calendar[dayOfYear]").
                port("x0").equals(70).
                port("x1").equals(365).
                port("ymin").equals(0).
                port("ymax").equals(16).
            endbox().
            box("PidController").name("controller").
                port("sensedValue").imports("..[value]").
                port("desiredValue").imports("outdoors[temperature]",CA).
                port("Kprop").equals(0.5e-4).
            endbox().
        endbox();
}

void Outdoors::reset() {
    update();
}

void Outdoors::update() {
    par = 4.57*propPar*radiation;
    ah = ahFromRh(temperature, rh);
    sh = shFromRh(temperature, rh);
}

} //namespace
