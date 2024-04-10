/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HEAT_BUFFER_H
#define HEAT_BUFFER_H
#include <base/box.h>


namespace vg {

class HeatBuffer : public base::Box
{
public: 
    HeatBuffer(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        volume, maxTemperature, U,
        inputResource, outputDemand,
        groundArea, externalTemperature, timeStep;
    // Outputs
    double
        inputDemand, inputSupply,
        outputResource, outputSupply,
        heatLoss, temperature;
};

} //namespace
#endif
