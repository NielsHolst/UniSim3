/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef GROWTH_LIGHTS_H
#define GROWTH_LIGHTS_H

#include "heat_transfer_layer_base.h"

namespace vg {

class GrowthLights : public HeatTransferLayerBase {
public:
    GrowthLights(QString name, Box *parent);
    void amend();
    void update();
private:
    // Inputs
    QVector<bool> lightsCurrentlyOn;
    QVector<double>
        lightsParEmittedDown,
        lightsSwEmittedDown,
        lightsLwFluxDown,
        lightsLwFluxUp,
        lightsPowerUsage;
    // Outputs
    bool currentlyOn;
    double powerUsage;
};

} //namespace


#endif
