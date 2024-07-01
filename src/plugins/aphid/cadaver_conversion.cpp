/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    // Hemmati et al 2001: winged S. avenae produce 20% less spores than apterous adult.
    // Let's assume that nymphs produce on average 50% less spores than apterous adult.
    cadavers = 0.5*(succumbedApterousNymphs + succumbedAlateNymphs) +
               0.8*succumbedAlateAdults + succumbedApterousAdults;
    count = succumbedApterousNymphs + succumbedAlateNymphs +
            succumbedAlateAdults + succumbedApterousAdults;
}

}


