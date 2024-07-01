/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
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
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    int numDays, timeStepSecs, stepWithinDay;
    double daysPassed, totalPar, sunPar;
    // Outputs
    double progressiveValue, value;
    // Data
    int _stepsPerDay;
    base::CircularBuffer<double> _lightSum;
    QVector<double> _sunPar;
    // Methods
    void fillInExpectedSunlight();
};

} //namespace
#endif
