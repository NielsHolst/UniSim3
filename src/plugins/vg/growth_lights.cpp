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
#include "growth_lights.h"

using namespace base;

namespace vg {

PUBLISH(GrowthLights)

GrowthLights::GrowthLights(QString name, Box *parent)
    : HeatTransferLayerBase(name, parent)
{
    help("sums power use and radiation from growth light");
    port("area")->imports("construction/geometry[groundArea]",CA);
    Input(lightsCurrentlyOn).imports("./*[currentlyOn]",CA);
    Input(lightsParFluxDown).imports("./*[parFluxDown]",CA);
    Input(lightsSwFluxDown). imports("./*[swFluxDown]",CA);
    Input(lightsLwFluxDown). imports("./*[lwFluxDown]",CA);
    Input(lightsLwFluxUp).   imports("./*[lwFluxDown]",CA);
    Input(lightsPowerUsage). imports("./*[powerUsage]",CA);
    Output(currentlyOn).help("Is any growth light on?");
    Output(powerUsage).help("Sum of growth lights' power usages");
}

void GrowthLights::amend() {
    BoxBuilder builder(this);
    QVector<Box*> children = findMany<Box*>("./*");
    if (children.isEmpty())
        builder.box("GrowthLight").name("noLight").endbox();
}

void GrowthLights::update() {
    currentlyOn = false;
    for (bool on : lightsCurrentlyOn)
        currentlyOn = (currentlyOn || on);
    parFluxDown = vector_op::sum(lightsParFluxDown);
    swFluxDown  = vector_op::sum(lightsSwFluxDown);
    lwFluxDown  = vector_op::sum(lightsLwFluxDown);
    lwFluxUp    = vector_op::sum(lightsLwFluxUp);
    powerUsage  = vector_op::sum(lightsPowerUsage);
}

} //namespace

