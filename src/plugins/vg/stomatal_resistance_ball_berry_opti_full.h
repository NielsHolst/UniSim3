/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef STOMATAL_RESISTANCE_BALL_BERRY_OPTI_FULL_H
#define STOMATAL_RESISTANCE_BALL_BERRY_OPTI_FULL_H

#include "stomatal_resistance_base.h"

namespace vg {

class StomatalResistanceBallBerryOptiFull : public StomatalResistanceBase
{
public:
    StomatalResistanceBallBerryOptiFull(QString name, Box *parent);

private:
    // Inputs
    double g0, g1, gk, rh, co2, temperature, Pg;
    // Methods
    double resetRsH2O();
    double updateRsH2O();
    double calcRsH2O(double co2);
};
} //namespace


#endif
