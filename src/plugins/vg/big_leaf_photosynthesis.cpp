/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include "big_leaf_photosynthesis.h"

using namespace base;

namespace vg {
	
PUBLISH(BigLeafPhotosynthesis)

BigLeafPhotosynthesis::BigLeafPhotosynthesis(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes simplified, big-leaf photosynthetic rate");
    Input(leafAn).imports("./leaf/leafPhotosynthesis[An]",CA);
    Input(leafAg).imports("./leaf/leafPhotosynthesis[Ag]",CA);
    Input(growthRespiration).equals(0.3).unit("[0;1]").help("Relative growth respiration");

    Output(An).help("Net big-leaf assimilation rate").unit("μmol CO2/m2 leaf/s");
    Output(Ag).help("Gross big-leaf assimilation rate").unit("μmol CO2/m2 leaf/s");
    Output(Ar).help("Big-leaf respiration rate").unit("μmol CO2/m2 leaf/s");

    Output(Pn).help("Net big-leaf growth rate").unit("g dry mass/m2 leaf/h");
    Output(Pg).help("Gross big-leaf growth rate").unit("g dry mass/m2 leaf/h");
    Output(Pr).help("Big-leaf respiration rate").unit("g dry mass/m2 leaf/h");
}

void BigLeafPhotosynthesis::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box*>("./leaf"))
        builder.
        box().name("leaf").
            box("IntercellularCo2").name("intercellularCo2").endbox().
            box("LeafParAbsorbed").name("parAbsorbed").endbox().
            box("LeafRespiration").name("respiration").endbox().
            box("LeafPhotosynthesisAj").name("Aj").endbox().
            box("StomatalResistanceBallBerry").name("stomatalResistance").endbox().
            box("LeafPhotosynthesis").name("leafPhotosynthesis").endbox().
        endbox();
}

void BigLeafPhotosynthesis::update() {
    // μmol CO2/m2 leaf/s
    An = leafAn;
    Ag = leafAg;
    Ar = Ag - An;
    // g CO2/m2 leaf/h
    double a = 3600*44.01*1e-6;
    Pn = An*a;
    Pg = Pg*a;
    Pr = Pr*a;
    // g biomass/m2 leaf/h
    double b = (1. - growthRespiration)*30./44.; // (Greenhouse Horticulture, p. 47)
    Pn *= b;
    Pg *= b;
    Pr *= b;
}

} //namespace

