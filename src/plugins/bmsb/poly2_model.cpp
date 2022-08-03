/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Byju N. Govindan, University of Minnesota [ngbyju at umn.edu].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
ï»¿#include <base/publish.h>
#include "poly2_model.h"
#include <math.h>

using namespace base;

namespace bmsb {

PUBLISH(Poly2Model)

Poly2Model::Poly2Model(QString name, QObject *parent)
    : Box(name, parent)
{
    help("estimate the adult pre-oviposition period using Poly2Model (Polynomial); later replace daily rate to hourly rate");
        Input(dailytempC).equals(17).help("Temperature (Â°C)");
        Input(a0).equals(228.61208).help("Estimate of coefficient a0)");
        Input(a1).equals(-15.04058).help("Estimate of coefficient a1");
        Input(a2).equals(0.25721).help("Estimate of coefficient a2");
        Output(p2moutput).help ("p2moutput refers to adult pre-oviposition period or longevity (days)");
}
void Poly2Model::reset() {
        update();
}

void Poly2Model::update() {
    help("hourly temperature will later replace daily temp to estimate parameter of interest based on poly2model");
    help("Note that input parameter must be supplied in degree Celsius for Temperature");
//      Poly2model as in devRate packege in R: (rT ~ a0 + a1 * T + a2 * T^2)
        p2moutput     = ((a0) +  (a1 * dailytempC) + (a2 * pow(dailytempC,2)));
}


}

