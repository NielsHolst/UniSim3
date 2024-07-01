/* Copyright 2016-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#include "primary_distribution_leaf.h"
#include <base/publish.h>
#include <base/vector_op.h>

using namespace base;

namespace PestTox {

PUBLISH (PrimaryDistributionLeaf)
	
PrimaryDistributionLeaf::PrimaryDistributionLeaf(QString name, Box *parent)
    : PrimaryDistributionBase(name, parent)
{
    help("manages pesticide deposition on crop or leaf surface");
    Input(fractionTakenAir).unit("g a.i/ha").imports("../surroundings[fractionTaken]").help("Dose taken by wind drift off-field edge");
    Input(stageContent).imports("crop/Stage::*[content]").help("Crop stage");
    Input(fractionsByCropStage).unit("fraction").help("Stage-specific fraction deposition");
}

void PrimaryDistributionLeaf::update() {
    double fractionLeft = 1. - fractionTakenAir;
    fractionTaken = vector_op::sumOfProducts(fractionsByCropStage, stageContent)*fractionLeft;
    doseTaken = doseApplied*fractionTaken;
}

} //namespace

