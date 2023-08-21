/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/box_builder.h>
#include <base/vector_op.h>
#include "average_cover_or_screen.h"

using vector_op::sum;

#define INPUT_MATERIALS(X) Input(X##s).imports(inputMaterials(#X)).help("Vector of materials' adjusted values")

QString inputMaterials(QString port) {
    return "./materials/*[" + port + "Adj]";
}

using namespace base;

namespace vg {

AverageCoverOrScreen::AverageCoverOrScreen(QString name, Box *parent)
    : Box(name, parent),
      LayerAdjusted(name, parent)
{
    help("computes average cover radiation and heat parameters");
    doWriteOnCommand(false);
    INPUT_MATERIALS(swAbsorptivityTop);
    INPUT_MATERIALS(swAbsorptivityBottom);
    INPUT_MATERIALS(swReflectivityTop);
    INPUT_MATERIALS(swReflectivityBottom);
    INPUT_MATERIALS(swTransmissivityTop);
    INPUT_MATERIALS(swTransmissivityBottom);

    INPUT_MATERIALS(lwAbsorptivityTop);
    INPUT_MATERIALS(lwAbsorptivityBottom);
    INPUT_MATERIALS(lwReflectivityTop);
    INPUT_MATERIALS(lwReflectivityBottom);
    INPUT_MATERIALS(lwTransmissivityTop);
    INPUT_MATERIALS(lwTransmissivityBottom);

    INPUT_MATERIALS(Utop);
    INPUT_MATERIALS(Ubottom);
    INPUT_MATERIALS(heatCapacity);
}

void AverageCoverOrScreen::reset() {
    update();
}

void AverageCoverOrScreen::update() {
    double a, r, t;
    a = sum(swAbsorptivityTops);
    r = sum(swReflectivityTops);
    t = sum(swTransmissivityTops);
    swAbsorptivityTopAdj   = a/(a+r+t);
    swReflectivityTopAdj   = r/(a+r+t);
    swTransmissivityTopAdj = t/(a+r+t);

    a = sum(swAbsorptivityBottoms);
    r = sum(swReflectivityBottoms);
    t = sum(swTransmissivityBottoms);
    swAbsorptivityBottomAdj   = a/(a+r+t);
    swReflectivityBottomAdj   = r/(a+r+t);
    swTransmissivityBottomAdj = t/(a+r+t);

    a = sum(lwAbsorptivityTops);
    r = sum(lwReflectivityTops);
    t = sum(lwTransmissivityTops);
    lwAbsorptivityTopAdj   = a/(a+r+t);
    lwReflectivityTopAdj   = r/(a+r+t);
    lwTransmissivityTopAdj = t/(a+r+t);

    a = sum(lwAbsorptivityBottoms);
    r = sum(lwReflectivityBottoms);
    t = sum(lwTransmissivityBottoms);
    lwAbsorptivityBottomAdj   = a/(a+r+t);
    lwReflectivityBottomAdj   = r/(a+r+t);
    lwTransmissivityBottomAdj = t/(a+r+t);

    UtopAdj = sum(Utops);
    updateUbottomAdj();
    heatCapacityAdj = sum(heatCapacitys);

    checkOutputs();
}

#define MATERIAL_PORT(X) port(#X).imports(pathToMaterialPorts(material, #X))

void AverageCoverOrScreen::buildAverageMaterial(BoxBuilder &builder, QString material) {
    QStringList myFaces = collectFacesByMaterial(material);
    builder.
    box("AverageMaterial").name(material).
        MATERIAL_PORT(swReflectivityTop).
        MATERIAL_PORT(swReflectivityBottom).
        MATERIAL_PORT(lwReflectivityTop).
        MATERIAL_PORT(lwReflectivityBottom).
        MATERIAL_PORT(swTransmissivityTop).
        MATERIAL_PORT(swTransmissivityBottom).
        MATERIAL_PORT(lwTransmissivityTop).
        MATERIAL_PORT(lwTransmissivityBottom).
        MATERIAL_PORT(swAbsorptivityTop).
        MATERIAL_PORT(swAbsorptivityBottom).
        MATERIAL_PORT(lwAbsorptivityTop).
        MATERIAL_PORT(lwAbsorptivityBottom).
        MATERIAL_PORT(Utop).
        MATERIAL_PORT(Ubottom).
        MATERIAL_PORT(heatCapacity).
        port("myAreas").  imports(pathToFaces(myFaces, "area")).
        port("myWeights").imports(pathToFaces(myFaces, "weight")).
    endbox();
}

QString AverageCoverOrScreen::pathToFaces(QStringList faces, QString port) const {
    QStringList result;
    for (const QString &face : faces) {
        result << "shelter/faces/" + face + "[" + port + "]";
    }
    return result.join("|");
}

} //namespace


