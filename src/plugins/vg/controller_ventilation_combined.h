/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CONTROLLER_VENTILATION_COMBINED_H
#define CONTROLLER_VENTILATION_COMBINED_H

#include <base/box.h>

namespace vg {

class ControllerVentilationCombined : public base::Box
{
public:
    ControllerVentilationCombined(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double controllerVentilation, controllerCrackVentilation, crackVentilation;
    // Output
    double ventilationChange, ventilationMax;
};
} //namespace


#endif
