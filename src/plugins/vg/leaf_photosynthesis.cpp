/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/exception.h>
#include <base/phys_math.h>
#include <base/publish.h>
#include "leaf_photosynthesis.h"

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(LeafPhotosynthesis)

LeafPhotosynthesis::LeafPhotosynthesis(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes canopy photosynthetic rate");
    Input(leafAj).imports("../Aj[value]");
    Input(leafAr).imports("../respiration[value]");
    Output(An).help("Net leaf assimilation rate").unit("μmol CO2/m2 leaf/s");
    Output(Ag).help("Gross leaf assimilation rate").unit("μmol CO2/m2 leaf/s");
    Output(Ar).help("Leaf respiration rate").unit("μmol CO2/m2 leaf/s");
}

void LeafPhotosynthesis::initialize() {
    Box *parent = findOne<Box*>("..");
    QString layerName = parent->name();
    if (layerName == "top")
        _wGauss = wGauss3[0];
    else if (layerName == "mid")
        _wGauss = wGauss3[1];
    else if (layerName == "bottom")
        _wGauss = wGauss3[2];
    else if (layerName == "leaf")
        _wGauss = 1.;
    else
        ThrowException("Expected parent called 'top', 'mid' or 'bottom'").value(layerName).context(this);
}

void LeafPhotosynthesis::update() {
    An = _wGauss*leafAj;
    Ag = _wGauss*(leafAj + leafAr);
    Ar = _wGauss*leafAr;
}

} //namespace

