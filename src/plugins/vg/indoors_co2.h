/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef INDOORS_CO2_H
#define INDOORS_CO2_H

#include <base/box.h>

namespace vg {

class IndoorsCo2 : public base::Box
{
public:
    IndoorsCo2(QString name, Box *parent);
    void reset();
    void update();

private:
    // Inputs
    double outdoorsCo2, airFlux,
        assimilation, indoorsTemperature,
        averageHeight, timeStep,
        injectionRate;
    // Outputs
    double value;
    // Methods
    double ppmToDensity(double ppm);
    double densityToPpm(double density);
};
} //namespace


#endif
