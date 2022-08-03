/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef INDOORS_HUMIDITY_H
#define INDOORS_HUMIDITY_H

#include <base/box.h>

namespace vg {

class IndoorsHumidity : public base::Box
{
public:
    IndoorsHumidity(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double initRh, conductance, gain, vapourFlux,
        temperature, timeStep, height;
    bool keepConstant;
    // Outputs
    double ah, rh, ahEq,
           netVapourFlux, timeConstant, surplusAh;

    // Data
    int _tick;
    double _constantAh;
};

} //namespace

#endif
