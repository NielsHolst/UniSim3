/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SUMMARY_H
#define SUMMARY_H

#include <base/box.h>

namespace vg {

class Summary : public base::Box
{
public:
    Summary(QString name, Box *parent);
    void update();
private:
    // Input
    double heatPipesHeatFlux, lampsPowerUsage, co2Injection, timeStep;
    bool isSkipping;
    // Output
    double heatingCost, lampCost, co2Cost;
};

} //namespace


#endif
