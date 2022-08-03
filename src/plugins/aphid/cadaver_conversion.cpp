/* Copyright 2018-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at nibio.no].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "cadaver_conversion.h"
#include <base/vector_op.h>

using vector_op::sum;
using namespace base;

namespace aphid {

PUBLISH(CadaverConversion)

CadaverConversion::CadaverConversion(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates the number of cadaver units based on the life stage of the aphid when becoming a cadaver");
    Input(succumbedApterousNymphs).help("New apterous nymph cadavers").unit("per tiller");
    Input(succumbedAlateNymphs).help("New alate nymph cadavers").unit("per tiller");
    Input(succumbedApterousAdults).help("New apterous adult cadavers").unit("per tiller");
    Input(succumbedAlateAdults).help("New alate adult cadavers").unit("per tiller");
    Output(cadavers).help("Cadavers in standardised units").unit("per tiller");
    Output(count).help("Number of cadavers").unit("per tiller");
}

void CadaverConversion::update() {
    // The inputs come as vectors which hold the age-distribution of how far the individuals came
    // in their development as aphids before turning into cadavers. That age distribution becomes
    // irrelevant once they have become cadaders.
    double apterousNymphs = sum(succumbedApterousNymphs),
           alateNymphs    = sum(succumbedAlateNymphs),
           apterousAdults = sum(succumbedApterousAdults),
           alateAdults    = sum(succumbedAlateAdults);
    cadavers = 0.5*(apterousNymphs + alateNymphs) + 0.8*alateAdults + apterousAdults;
    // Hemmati et al 2001: winged S. avenae produce 20% less spores than apterous adult.
    // let's assume that nymphs produce on average 50% less spores than apterous adult.
    count = apterousNymphs + alateNymphs + alateAdults + apterousAdults;
}

}


