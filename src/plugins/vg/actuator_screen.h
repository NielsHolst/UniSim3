/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef ACTUATOR_SCREEN_H
#define ACTUATOR_SCREEN_H
#include <base/box.h>
#include "layer_adjusted.h"
#include "shelter_face.h"

namespace vg {

class ActuatorScreen : public LayerAdjusted
{
public: 
    ActuatorScreen(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    QVector<QString> shelterFaceScreens;
    double roofArea, sideArea, endArea, coverArea, groundArea;
    // Outputs
    // Data
    static QMap<ShelterFace, double> _areas;
    base::Box *_screen;
    QVector<ShelterFace> _allShelterFaces, _shelterFacesWithScreen;
    // Methods
    double weighted(QString parameter, double faceArea) {
        return _screen->port(parameter)->value<double>()*faceArea;
    }
};

} //namespace
#endif
