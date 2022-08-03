/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef HEAT_TRANSFER_PIPE_H
#define HEAT_TRANSFER_PIPE_H
#include "heat_transfer_layer_base.h"

namespace vg {

class HeatTransferPipe : public HeatTransferLayerBase
{
public:
    HeatTransferPipe(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double energyFluxRadiation;
};

} //namespace


#endif
