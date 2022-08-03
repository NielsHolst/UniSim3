/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SCREEN_H
#define SCREEN_H
#include "heat_transfer_layer_parameters.h"
#include <base/box.h>

namespace vg {

class Screen : public HeatTransferLayerParameters
{
public:
    Screen(QString name, Box *parent);
    void reset();
    void update();
private:
    friend class Screens;

    // Inputs
    double haze, transmissivityAir, state,
        ventilation,
        Uair, UstateExponent, UventilationSlope;
    // Outputs
    double
        swReflectivityTopNet, swReflectivityBottomNet,
        swTransmissivityTopNet, swTransmissivityBottomNet,
        lwReflectivityTopNet, lwReflectivityBottomNet,
        lwTransmissivityTopNet, lwTransmissivityBottomNet,
        swAbsorptivityTopNet, swAbsorptivityBottomNet,
        lwAbsorptivityTopNet, lwAbsorptivityBottomNet,
        emissivityTopNet, emissivityBottomNet,
        UtopNet, UbottomNet,
        maxArea;
//        heatCapacity;
    // Methods
    void updateByState(double state);
    void updateU();
    virtual double computeMaxArea() = 0;
};

} //namespace


#endif
