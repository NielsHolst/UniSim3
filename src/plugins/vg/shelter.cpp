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
#include "shelter.h"
#include <base/vector_op.h>

using std::max;
using namespace base;

namespace vg {

PUBLISH(Shelter)

Shelter::Shelter(QString name, Box *parent)
    : Box(name, parent)
{
    help("models the greenhouse shelter");
    Input(screenAirExponent).equals(4.).unit("-").help("Exponent for screen air transmissivity vs. state");
    Input(Uair).equals(2.27).unit("W/K/m2 screen").help("U-value for air fully enclosed between two screens or screens/cover");
    Input(screenUstateExponent).equals(32.).unit("-").help("Exponent for screen U vs. state");
    Input(screenUventilationSlope).equals(2.).unit("-").help("Exponent for screen U vs. ventilation");
    Input(airTransmissivities).imports("./*/screens/airTransmissivity[value]", CA).
            help("Air transmissivity through screens for each shelter face");
    Input(areas).imports("./*/area[value]", CA).help("Area of each shelter face");
    Input(screenPerfection).equals(0.98).unit("[0;1]").help("How perfect are the screens covering the shelter when fully drawn?");
    Output(screensAirTransmissivity).help("Total net transmissivity of screens").unit("[0;1]");
}

void Shelter::amend() {
    BoxBuilder builder(this);
    amendShelter(builder, "roof1");
    amendShelter(builder, "roof2");
    amendShelter(builder, "side1");
    amendShelter(builder, "side2");
    amendShelter(builder, "end1");
    amendShelter(builder, "end2");
}

void Shelter::amendShelter(BoxBuilder &builder, QString shelterName) {
    if (!findMaybeOne<Box*>("./" + shelterName))
        builder.box("ShelterFace").name(shelterName).endbox();
}

void Shelter::update() {
    screensAirTransmissivity = airTransmissivities.isEmpty() ? 0. : vector_op::weightedAverage(airTransmissivities, areas, this);
}

} //namespace

