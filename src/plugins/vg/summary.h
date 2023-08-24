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
    void reset();
    void update();
private:
    // Input
    double heatPipesHeatFlux, lampsPowerUsage, co2Injection,
        transpirationFinite, condensationFinite, ventedWaterFinite,
        screenState1, screenState2, screenState3,
        lightState1, lightState2, lightState3;
    bool isSkipping;
    // Output
    double heatingCost, lampCost, co2Cost,
        transpiration, condensation, ventedWater,
        screen1, screen2, screen3,
        light1, light2, light3;
    // Data
    double _heatingSum, _lampSum, _co2Sum,
        _screenState1Sum, _screenState2Sum, _screenState3Sum,
        _lightState1Sum, _lightState2Sum, _lightState3Sum,
        _transpiration, _condensation, _ventedWater;
    int _n;
};

} //namespace


#endif
