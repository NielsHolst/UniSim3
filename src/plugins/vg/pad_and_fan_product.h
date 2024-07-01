/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PAD_AND_FAN_PRODUCT_H
#define PAD_AND_FAN_PRODUCT_H
#include <base/box.h>


namespace vg {

class PadAndFanProduct : public base::Box
{
public: 
    PadAndFanProduct(QString name, Box *parent);
private:
    // Inputs
    double coolingEfficiency, powerUseMax, flowRateMax, powerUserParasitic;
};

} //namespace
#endif
