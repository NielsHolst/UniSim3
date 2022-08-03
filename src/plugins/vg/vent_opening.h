/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef VENT_OPENING_H
#define VENT_OPENING_H

#include <base/box.h>

namespace vg {

class VentOpening : public base::Box
{
public:
    VentOpening(QString name, Box *parent);
    void update();
private:
    // Inputs;
    double airFluxSupply, airFluxSupplyMax, ventOpeningExponent;
    // Outputs
    double value;
};
} //namespace


#endif
