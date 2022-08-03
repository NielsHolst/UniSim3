/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LEAF_PAR_ABSORBED_H
#define LEAF_PAR_ABSORBED_H

#include <base/box.h>

namespace vg {

class LeafParAbsorbed : public base::Box
{
public:
    LeafParAbsorbed(QString name, Box *parent);
    void initialize();
    void update();
private:
    // Inputs
    double parFlux, k, lai;
    // Outputs
    double value;
    // Data
    double _xGauss;
};
} //namespace


#endif
