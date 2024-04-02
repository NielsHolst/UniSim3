/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PLANT_OPTIMISER_H
#define PLANT_OPTIMISER_H
#include <base/box.h>

namespace base {
    class GradientAscent;
}

namespace vg {

class Plant;

class PlantOptimiser : public base::Box
{
public: 
    PlantOptimiser(QString name, Box *parent);
    void initialize();
    void reset();
    void update();
private:
    // Inputs
    double minTemperature, maxTemperature, minCo2, maxCo2, delta;
    int maxIterations;
    // Outputs
    double temperature, co2, growthRate;
    int iterations;
    // Data
    base::GradientAscent optimiser;
public:
    static Plant *plant;
};

} //namespace
#endif
