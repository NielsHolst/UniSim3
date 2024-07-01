/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef OUTDOORS_H
#define OUTDOORS_H

#include <base/box.h>

namespace vg {

class Outdoors : public base::Box
{
public:
    Outdoors(QString name, Box *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    double temperature, windSpeed, rh,
    radiation, propPar, propUv,
    co2, soilTemperature;

    // Outputs
    double par;

};
} //namespace


#endif
