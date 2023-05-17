/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GROWTH_LIGHTS_H
#define GROWTH_LIGHTS_H

#include "layer_adjusted.h"

namespace vg {

class GrowthLights : public LayerAdjusted {
public:
    GrowthLights(QString name, base::Box *parent);
    void reset();
    void update();
private:
    // Inputs
    QVector<double>
        parEmissionBottomLights,
        swEmissionBottomLights,
        lwEmissionBottomLights, lwEmissionTopLights,
        convectionBottomLights, convectionTopLights,
        powerUsageLights;
    // Outputs
    double
        parEmissionBottom,
        swEmissionBottom,
        lwEmissionBottom, lwEmissionTop,
        convectionBottom, convectionTop,
        powerUsage;
};

} //namespace


#endif
