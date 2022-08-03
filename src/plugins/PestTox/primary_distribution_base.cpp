/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "primary_distribution_base.h"
#include <cmath>
#include <base/publish.h>


using namespace base;

namespace PestTox {

PrimaryDistributionBase::PrimaryDistributionBase(QString name, Box *parent)
	: Box(name, parent)
{
    help("manages primary distribution");
    Input(fieldArea).unit("m2").imports("field[area]").help("Area of whole field");
    Input(doseApplied).unit("g a.i/ha").imports("applications[dose]").help("Dose of pesticide");
    Output(doseTaken).unit("g a.i/ha").help("Dose taken by this compartment");
    Output(fractionTaken).unit("fraction").help("Fraction of applied dose taken by this compartment [0;1]");
}


} //namespace

