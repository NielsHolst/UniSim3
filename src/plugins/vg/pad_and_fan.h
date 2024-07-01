/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PAD_AND_FAN_H
#define PAD_AND_FAN_H
#include <base/box.h>


namespace vg {

class PadAndFan : public base::Box
{
public: 
    PadAndFan(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double groundArea, outdoorsTemperature, outdoorsRh, efficiency, maxMassFlowRate,
        state;
    // Outputs
    double coolingPower, vapourFlux;
};

} //namespace
#endif
