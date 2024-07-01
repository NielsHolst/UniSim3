/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef GROWTH_LIGHT_CONTROLLER_H
#define GROWTH_LIGHT_CONTROLLER_H
#include <base/box.h>

namespace vg {

class GrowthLightController : public base::Box
{
public:
    GrowthLightController(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    int mode;
    double thresholdLow, thresholdHigh,
        desiredLightSum, currentLightSum,
        input,
        minPeriodOn, timeStepSecs;
    bool atMidnight;
    // Outputs
    bool isOn, switchOffForToday;
    double periodOn;
    // Data
    enum class Mode{Off=0, Threshold=1, LightSum=2, On=10};
    // Methods
    void switchByThreshold();
    void switchByLightSum();
};
} //namespace


#endif
