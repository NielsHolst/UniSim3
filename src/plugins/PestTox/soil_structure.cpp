/* Copyright 2016-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Mwema Felix, Tropical Pesticides Research Institute, Tanzania [mwema.mwema at tpri.go.tz.].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include "soil_structure.h"
#include <base/publish.h>
#include <base/test_num.h>

using namespace base;

namespace PestTox {

PUBLISH (SoilStructure)
	
SoilStructure::SoilStructure(QString name, Box *parent)
	: Box(name, parent)
{

    help("manages soil porosity");
    Input(fa).unit("fraction").equals(0.25).help("Fraction of air in the soil");
    Input(fw).unit("fraction").equals(0.25).help("Fraction of water in the soil");

    Output(fs).unit("fraction").help("Fraction of sand in the soil");
    Output(porosity).help("Soil porosity");
}

void SoilStructure::reset() {
    update();
}

void SoilStructure::update() {
    porosity = fa + fw;
    fs = 1. - porosity;

    double sum = fa+fw+fs;
    if (TestNum::ne(sum, 1.))
        ThrowException("Sum of soil fractions must equal 1").value(sum).context(this);
}

} //namespace

