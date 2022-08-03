/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef LEAF_LIGHT_RESPONSE_DEPRECATED_H
#define LEAF_LIGHT_RESPONSE_DEPRECATED_H


#include <base/box.h>

namespace vg {

class LeafLightResponseDeprecated : public base::Box
{
public:
    LeafLightResponseDeprecated(QString name, Box *parent);
    void amend();
    void reset();
    void update();

private:
    // Inputs
    double rsCO2, rbCO2, Tleaf, co2Air,
           theta, frParAbs,
           VCmax, Jmax, KM, gamma;
    // Outputs
    double rtCO2, Pnmax, Pgmax, lue, Rd;
    // Methods
    double darkRespirationRate();
    double potentialLightUseEfficiency();    
    double maxNetAssimilation();
};
} //namespace


#endif
