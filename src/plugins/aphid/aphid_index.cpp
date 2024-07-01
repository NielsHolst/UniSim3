/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include <base/publish.h>
#include "aphid_index.h"

using namespace base;

namespace aphid {

PUBLISH(AphidIndex)

AphidIndex::AphidIndex(QString name, Box *parent)
    : Box(name, parent)
{
    help("calculates Entwistle-Dixon-Wratten density index ");
    Input(nymphs).help("Aphid nymph density \\((N_{nymph})\\)").unit("per tiller");
    Input(adults).help("Aphid adult density \\((N_{adult})\\)").unit("per tiller");
    Output(value).help("Index value \\(y\\)");
}

void AphidIndex::reset() {
    update();
}

void AphidIndex::update() {
    value = log(0.5*nymphs + adults + 0.01) ;
}

}
