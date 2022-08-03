/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Antonio Agüera García [antonio.aguera at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/

/* MUSSEL GROWTH SCALING: Scale maximum growth rate by current step temperature,
   this scaling function is based in the data from Almeda-Villela et al., 1982, Fig 2. */

#include <math.h>
#include <base/publish.h>
#include "mussel_temp_scale.h"

using namespace base;

namespace MusselBed {

PUBLISH(MusselTempScale)

MusselTempScale::MusselTempScale(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(a).equals(0.408).help("Regression coefficient");
    Input(b).equals(-4.157).help("Regression coefficient");
    Input(temperature).equals(12).help("current step temperature in degree Celsius");
    Output(value).help("scaling parameter at current temperature");
}

void MusselTempScale::reset() {
    update();
}

void MusselTempScale::update() {
//    value =(exp(0.40831*temperature - 4.15734)/(1+exp(0.40831*temperature - 4.15734)));
    value = exp(a*temperature + b)/(1.+exp(a*temperature + b));
    }


}//namespace


