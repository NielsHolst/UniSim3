/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GROWTH_LIGHT_CONTROLLER_H
#define GROWTH_LIGHT_CONTROLLER_H
#include <base/base_signal.h>

namespace vg {

class GrowthLightController : public base::BaseSignal
{
public:
    GrowthLightController(QString name, Box *parent);
    bool computeFlag();
    double computeSignal(bool flag);
private:
    // Inputs
    int setting;
    double lightThresholdLow, lightThresholdHigh, lightOutdoors;
    // Data
    bool _isOn;
};
} //namespace


#endif
