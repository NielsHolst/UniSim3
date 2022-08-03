/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include <base/test_num.h>
#include <base/phys_math.h>
#include "leaf_photosynthesis_deprecated.h"

using namespace base;
using namespace phys_math;

namespace vg {
	
PUBLISH(LeafPhotosynthesisDeprecated)

LeafPhotosynthesisDeprecated::LeafPhotosynthesisDeprecated(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes single-leaf light capture and photosynthetic rate");
    Input(parAbsorbed).unit("umol/m2/s").help("Absorbed PAR");
    Input(Pgmax).imports("./lightResponse[Pgmax]",CA);
    Input(lue).imports("./lightResponse[lue]",CA);
    Input(Rd).imports("./lightResponse[Rd]",CA);
    Output(Pg).help("Gross assimilation rate").unit("mg CO2/m2/s");
    Output(Pn).help("Net assimilation rate").unit("mg CO2/m2/s");
}

void LeafPhotosynthesisDeprecated::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box*>("./lightResponse"))
        builder.
        box("vg::LeafLightResponse").name("lightResponse").
        endbox();
}

void LeafPhotosynthesisDeprecated::update() {
    Pg = (Pgmax > 0) ? Pgmax*(1.-exp(-parAbsorbed*lue/Pgmax)) : 0.;
    Pn = Pg - Rd;
}

} //namespace

