/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "sheep_fecundity.h"

using namespace base;

namespace rvf {

PUBLISH(SheepFecundity)

SheepFecundity::SheepFecundity(QString name, QObject *parent)
	: Box(name, parent)
{
    Input(lambDensity).help("Density of lambs").unit("km-2");
    Input(adultDensity).help("Density of adult sheep").unit("km-2");
    Input(sexRatio).equals(0.5).help("Proportion of females").unit("[0;1]");
    Input(birthRate).help("Lambs produced per female per year").unit("y-1");
    Input(carryingCapacity).help("Maximum sheep density").unit("km-2");
    Output(value);
}

void SheepFecundity::update() {
    // In the original model, only adults counted towards carrying capacity
    double totalDensity = adultDensity + lambDensity;
    value = birthRate/365*adultDensity*sexRatio*(carryingCapacity - totalDensity)/carryingCapacity;
    // Correct for numerical imprecision near carrying capacity
    if (value < 0.)
        value = 0.;
}


} //namespace

