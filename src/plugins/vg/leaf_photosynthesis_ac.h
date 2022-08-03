/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CANOPY_PHOTOSYNTHESIS_AC_H
#define CANOPY_PHOTOSYNTHESIS_AC_H

#include <base/box.h>

namespace vg {

class LeafPhotosynthesisAc : public base::Box
{
public:
    LeafPhotosynthesisAc(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double
        Vcmax, gammastar, respiration, Km,
        EaV, EdVC, delsC,
        leafTemperature, intercellularCo2;
    // Outputs
    double VcmaxTemp, value;
};
} //namespace


#endif
