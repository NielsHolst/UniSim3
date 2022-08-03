/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "compartment.h"
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>

using namespace base;

namespace PestTox {

PUBLISH(Compartment)
	
Compartment::Compartment(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages the current load of active ingredient in a compartment");
    Input(gain).unit("g a.i. per ha").help("Daily gain");
    Input(loss).unit("g a.i. per ha").help("Daily loss");

    Output(load).unit("g a.i. per ha").help("Current pesticide load");
}

void Compartment::update() {
    load += gain - loss;
    TestNum::snapToZero(load, 1e-18);
    if (load < 0.)
        ThrowException("Load cannot be negative").value(load).context(this);
}

} //namespace

