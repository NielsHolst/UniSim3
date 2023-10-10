/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LIGHT_SUM_H
#define LIGHT_SUM_H
#include <base/box.h>
#include <base/circular_buffer.h>

namespace vg {

class LightSum : public base::Box
{
public: 
    LightSum(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    int numDays, stepWithinDay;
    double timeStep, parIntensity;
    // Outputs
    double value;
    // Data
    base::CircularBuffer<double> _buffer;
};

} //namespace
#endif
