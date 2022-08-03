/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#include <base/publish.h>
#include "controller_ventilation_combined.h"

using namespace base;

namespace vg {
	
PUBLISH(ControllerVentilationCombined)

ControllerVentilationCombined::ControllerVentilationCombined(QString name, Box *parent)
	: Box(name, parent)
{
    help("combines controllers for ventilation");
    Input(controllerVentilation).imports("controllers/ventilation[value]", CA);
    Input(controllerCrackVentilation).imports("controllers/crackVentilation[value]", CA);
    Input(crackVentilation).imports("setpoints/elementary/crackVentilationNormalTemperature[value]", CA);
    Output(ventilationChange).help("Reconciled change in opening");
    Output(ventilationMax).help("Reconciled max. opening").unit("[0;1]");
}

void ControllerVentilationCombined::reset() {
    update();
}

void ControllerVentilationCombined::update() {
    ventilationChange = (controllerVentilation < 0. && controllerCrackVentilation < 0.) ?
            // If both want to close, let the largest desire win
            std::min(controllerVentilation, controllerCrackVentilation) :
            // If either or both want to open, let the largest desire win
            std::max(controllerVentilation, controllerCrackVentilation);

    ventilationMax = (controllerVentilation < 0. && controllerCrackVentilation > 0.) ?
            // If there is a desire for crack only then the max. opening is restricted
            crackVentilation :
            // Otherwise the windows are allowed to open fully
            1.;

}

} //namespace
