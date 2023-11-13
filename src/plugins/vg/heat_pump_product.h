/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HEAT_PUMP_PRODUCT_H
#define HEAT_PUMP_PRODUCT_H
#include <base/box.h>


namespace vg {

class HeatPumpProduct : public base::Box
{
public: 
    HeatPumpProduct(QString name, Box *parent);
private:
    // Inputs
    double maxCoolingLoad, coolingEfficiency, maxFlowRate, parasiticLoad, coolingTemperature;
};

} //namespace
#endif
