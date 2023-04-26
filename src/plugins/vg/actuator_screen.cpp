/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/exception.h>
#include <base/publish.h>
#include "actuator_screen.h"

using namespace base;

namespace vg {

PUBLISH(ActuatorScreen)

QMap<ShelterFace, double> ActuatorScreen::_areas;

ActuatorScreen::ActuatorScreen(QString name, Box *parent)
    : Box(name, parent), LayerAdjusted(name, parent)
{
    help("models the screens in layer 1, 2 or 3");
    Input(shelterFaceScreens).imports("shelter/faces/*[screens]");
    Input(shelterFaceWeights).imports("shelter/faces/*[weight]");
    Input(roofArea).imports("construction/geometry[roofArea]");
    Input(sideArea).imports("construction/geometry[sideArea]");
    Input(endArea).imports("construction/geometry[endArea]");
    Input(coverArea).imports("construction/geometry[coverArea]");

}

void ActuatorScreen::initialize() {
    // Set areas of shelter faces
    _areas[ShelterFace::Roof1] =
    _areas[ShelterFace::Roof2] = roofArea/2.;
    _areas[ShelterFace::Side1] =
    _areas[ShelterFace::Side2] = sideArea/2.;
    _areas[ShelterFace::End1]  =
    _areas[ShelterFace::End2]  = endArea/2. ;

    // Create a list of the shelter faces in the order they are defined
    QVector<Box*> shelterFaces = findMany<Box*>("shelter/faces/*");
    auto m = shelterFaces.size();
    if (m != 6)
        ThrowException("Shelter must hold 6 faces").value(m).context(this);
    for (Box *shelterFace : shelterFaces) {
        QString name = shelterFace->objectName();
        _allShelterFaces << toShelterFace(name, this);
    }

    // Find box with my screen
    const QString screenName = objectName();
    _screen = findOne<Box*>("shelter/screens/" + screenName);

    // Decode screen formulas and collect a list of the shelter faces where my screen is present
    auto n = shelterFaceScreens.size();
    if (n != 6)
        ThrowException("A screen formula must be specified for each shelter face").value(n).context(this);
    for (int i=0; i<6; ++i) {
        QStringList screensOnShelterFace = shelterFaceScreens.at(i).split("+");
        if (screensOnShelterFace.contains(screenName))
            _shelterFacesWithScreen << _allShelterFaces.at(i);
    }
}

void ActuatorScreen::reset() {
    update();
}

#define ADD_WEIGHTED(X) X += weighted(#X, faceArea)

void ActuatorScreen::update() {
    // Weigh each of my screen's parameters with the area of the faces where it is present
    swReflectivityTopAdj =
    swReflectivityBottomAdj =
    swTransmissivityTopAdj =
    swTransmissivityBottomAdj =
    lwReflectivityTopAdj =
    lwReflectivityBottomAdj =
    lwTransmissivityTopAdj =
    lwTransmissivityBottomAdj =
    swAbsorptivityTopAdj =
    swAbsorptivityBottomAdj =
    lwAbsorptivityTopAdj =
    lwAbsorptivityBottomAdj =
    UtopAdj =
    UbottomAdj =
    heatCapacityAdj = 0;
    for (ShelterFace face : _allShelterFaces) {
        const double faceArea = _areas.value(face);
        if (_shelterFacesWithScreen.contains(face)) {
            ADD_WEIGHTED(swReflectivityTopAdj);
            ADD_WEIGHTED(swReflectivityBottomAdj);
            ADD_WEIGHTED(swTransmissivityTopAdj);
            ADD_WEIGHTED(swTransmissivityBottomAdj);
            ADD_WEIGHTED(lwReflectivityTopAdj);
            ADD_WEIGHTED(lwReflectivityBottomAdj);
            ADD_WEIGHTED(lwTransmissivityTopAdj);
            ADD_WEIGHTED(lwTransmissivityBottomAdj);
            ADD_WEIGHTED(swAbsorptivityTopAdj);
            ADD_WEIGHTED(swAbsorptivityBottomAdj);
            ADD_WEIGHTED(lwAbsorptivityTopAdj);
            ADD_WEIGHTED(lwAbsorptivityBottomAdj);
            ADD_WEIGHTED(UtopAdj);
            ADD_WEIGHTED(UbottomAdj);
            ADD_WEIGHTED(heatCapacityAdj);
        }
        else {
            swTransmissivityTopAdj    += faceArea;
            lwTransmissivityTopAdj    += faceArea;
            swTransmissivityBottomAdj += faceArea;
            lwTransmissivityBottomAdj += faceArea;
        }
    }
    // For radiative parameters use the weighted average
    swReflectivityTopAdj /= coverArea;
    swReflectivityBottomAdj /= coverArea;
    swTransmissivityTopAdj /= coverArea;
    swTransmissivityBottomAdj /= coverArea;
    lwReflectivityTopAdj /= coverArea;
    lwReflectivityBottomAdj /= coverArea;
    lwTransmissivityTopAdj /= coverArea;
    lwTransmissivityBottomAdj /= coverArea;
    swAbsorptivityTopAdj /= coverArea;
    swAbsorptivityBottomAdj /= coverArea;
    lwAbsorptivityTopAdj /= coverArea;
    lwAbsorptivityBottomAdj /= coverArea;
    // For others convert to per ground area
    UtopAdj /= groundArea;
    UbottomAdj /= groundArea;
    heatCapacityAdj /= groundArea;
}

} //namespace

