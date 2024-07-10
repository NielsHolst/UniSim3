/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef HEAT_PUMPS_MAX_STATE_H
#define HEAT_PUMPS_MAX_STATE_H
#include <base/box.h>


namespace vg {

class HeatPumpsMaxState : public base::Box
{
public: 
    HeatPumpsMaxState(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector<double> maxCoolingLoads;
    QVector<int> numbers;
    double mode, maxPowerUse;
    // Outputs
    double value;
    // data
    double _maxCoolingLoadsTotal;
    bool _firstUpdate;
};

} //namespace
#endif
