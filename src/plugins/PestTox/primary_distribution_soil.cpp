/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "primary_distribution_soil.h"
#include <cmath>
#include <base/exception.h>
#include <base/publish.h>
#include <base/test_num.h>

using namespace base;

namespace PestTox {

PUBLISH (PrimaryDistributionSoil)
	
PrimaryDistributionSoil::PrimaryDistributionSoil(QString name, Box *parent)
    : PrimaryDistributionBase(name, parent)
{

    help("Computes pesticide deposition in the topsoil");
    Input(fractionTakenSurroundings).unit("(g a.i/ha)").imports("../surroundings[fractionTaken]").help("Dose taken by wind drift off-field edge");
    Input(fractionTakenCrop).unit("g a.i/ha").imports("../crop[fractionTaken]").help("Dose deposited on the crop or leaf surface");
}

void PrimaryDistributionSoil::update() {
    double alreadyTaken = fractionTakenSurroundings + fractionTakenCrop ;
    bool withinBounds = (TestNum::geZero(alreadyTaken) && TestNum::le(alreadyTaken, 1.));
    if (!withinBounds)
        ThrowException("Already taken fraction should be inside [0;1]").
                value(alreadyTaken).context(this);

    fractionTaken = 1. - alreadyTaken;
    doseTaken = doseApplied*fractionTaken;
}

} //namespace

