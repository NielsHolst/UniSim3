/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef HEAT_PUMPS_H
#define HEAT_PUMPS_H
#include <base/box.h>

namespace vg {

class HeatPumps : public base::Box {
public:
    HeatPumps(QString name, base::Box *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector<double>
        powerUsePumps,
        coolingPumps,
        energyToBufferPumps,
        condensationPumps;
    // Outputs
    double
        powerUse,
        cooling,
        energyToBuffer,
        condensation;
};

} //namespace


#endif
