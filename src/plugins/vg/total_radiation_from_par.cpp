/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "total_radiation_from_par.h"

using namespace base;

namespace vg {

PUBLISH(TotalRadiationFromPar)

TotalRadiationFromPar::TotalRadiationFromPar(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes total radiation from par");
    Input(par).help("PAR").unit("umol");
    Input(factor).equals(2.1).help("Converting PAR to total radiation").unit("J/umol");
    Output(value).help("Total radiation").unit("J");
}

void TotalRadiationFromPar::reset() {
    update();
}

void TotalRadiationFromPar::update() {
    value = par*factor;
}

} //namespace

