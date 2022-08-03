/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef COVER_H
#define COVER_H
#include <base/box.h>
#include "heat_transfer_layer_parameters.h"

namespace vg {

class Cover : public HeatTransferLayerParameters
{
public:
    Cover(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        UwindMinimum, UwindSlope, UwindExponent, windSpeed, haze;
};
} //namespace


#endif
