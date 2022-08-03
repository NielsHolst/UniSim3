/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LEAF_RESPIRATION_H
#define LEAF_RESPIRATION_H

#include <base/box.h>

namespace vg {

class LeafRespiration : public base::Box
{
public:
    LeafRespiration(QString name, Box *parent);
    void reset();
    void update();

private:
    // Inputs
    double Q10, RL, leafTemperature,
        canopyParAbsorbed, darknessThreshold;
    // Outputs
    double value;
};
} //namespace


#endif
