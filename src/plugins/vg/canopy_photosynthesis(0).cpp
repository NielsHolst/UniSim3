/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/publish.h>
#include <base/vector_op.h>
#include "canopy_photosynthesis.h"

using namespace base;

namespace vg {
	
PUBLISH(CanopyPhotosynthesis)

CanopyPhotosynthesis::CanopyPhotosynthesis(QString name, Box *parent)
    : Box(name, parent)
{
    help("computes canopy photosynthetic rate");
    Input(lai).imports("/crop[lai]",CA);
    Input(leafAn).imports("./*/leafPhotosynthesis[An]",CA);
    Input(leafAg).imports("./*/leafPhotosynthesis[Ag]",CA);
    Input(growthRespiration).equals(0.3).unit("[0;1]").help("Relative growth respiration");
    Input(trackPn).imports("..[trackPn]");

    Output(An).help("Net canopy assimilation rate").unit("&micro;mol CO2/m2 leaf/s");
    Output(Ag).help("Gross canopy assimilation rate").unit("&micro;mol CO2/m2 leaf/s");
    Output(Ar).help("Canopy respiration rate").unit("&micro;mol CO2/m2 leaf/s");

    Output(Pn).help("Net canopy growth rate").unit("g dry mass/ground m2/h");
    Output(Pg).help("Gross canopy growth rate").unit("g dry mass/ground m2/h");
    Output(Pr).help("Canopy respiration rate").unit("g dry mass/ground m2/h");

    Output(trackedPn).help("Pn values if 'trackPn' is true");
}

void CanopyPhotosynthesis::amend() {
    BoxBuilder builder(this);
    if (!findMaybeOne<Box*>("./top"))
        builder.
        box().name("top").
            box("IntercellularCo2").name("intercellularCo2").endbox().
            box("LeafParAbsorbed").name("parAbsorbed").endbox().
            box("LeafRespiration").name("respiration").endbox().
            box("LeafPhotosynthesisAj").name("Aj").endbox().
            box("StomatalResistanceBallBerry").name("stomatalResistance").endbox().
            box("LeafPhotosynthesis").name("leafPhotosynthesis").endbox().
        endbox().
        box().name("mid").
            box("IntercellularCo2").name("intercellularCo2").endbox().
            box("LeafParAbsorbed").name("parAbsorbed").endbox().
            box("LeafRespiration").name("respiration").endbox().
            box("LeafPhotosynthesisAj").name("Aj").endbox().
            box("StomatalResistanceBallBerry").name("stomatalResistance").endbox().
            box("LeafPhotosynthesis").name("leafPhotosynthesis").endbox().
        endbox().
        box().name("bottom").
            box("IntercellularCo2").name("intercellularCo2").endbox().
            box("LeafParAbsorbed").name("parAbsorbed").endbox().
            box("LeafRespiration").name("respiration").endbox().
            box("LeafPhotosynthesisAj").name("Aj").endbox().
            box("StomatalResistanceBallBerry").name("stomatalResistance").endbox().
            box("LeafPhotosynthesis").name("leafPhotosynthesis").endbox().
        endbox();
}

void CanopyPhotosynthesis::reset() {
    trackedPn.clear();
}

void CanopyPhotosynthesis::update() {
    // &micro;mol CO2/m2 leaf/s
    An = vector_op::sum(leafAn);
    Ag = vector_op::sum(leafAg);
    Ar = Ag - An;
    // &micro;mol CO2/m2 ground/s
    Pn = An*lai;
    Pg = Ag*lai;
    Pr = Ar*lai;
    // g CO2/ground m2/h
    double a = 3600*44.01*1e-6;
    Pn *= a;
    Pg *= a;
    Pr *= a;
    // g biomass/ground m2/h
    double b = (1. - growthRespiration)*30./44.; // (Greenhouse Horticulture, p. 47)
    Pn *= b;
    Pg *= b;
    Pr *= b;
    // Track?
    if (trackPn)
        trackedPn << Pn;
}

} //namespace

