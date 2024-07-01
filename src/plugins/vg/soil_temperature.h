/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef SOIL_TEMPERATURE_H
#define SOIL_TEMPERATURE_H
#include <base/box.h>
#include <base/circular_buffer.h>

namespace vg {

class SoilTemperature : public base::Box
{
public: 
    SoilTemperature(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        outdoorsTemperature,
        initial, min, max;
    bool atMidnight;
    // Outputs
    double value;
    // Data
    const int _bufSize = 7;
    base::CircularBuffer<double> _buf;
    double _dailySum;
    int _steps;
};

} //namespace
#endif
