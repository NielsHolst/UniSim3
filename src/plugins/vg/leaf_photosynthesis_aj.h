/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef CANOPY_PHOTOSYNTHESIS_AJ_H
#define CANOPY_PHOTOSYNTHESIS_AJ_H

#include <base/box.h>

namespace vg {

class LeafPhotosynthesisAj : public base::Box
{
public:
    LeafPhotosynthesisAj(QString name, Box *parent);
    void update();
private:
    // Inputs
    double
        gammaStar, Jmax, k,
        leafTemperature, intercellularCo2, parAbsorbed;
    // Outputs
    double JmaxTemp, J, value, respiration;
    // Methods
    void setJmaxTemp();
};
} //namespace


#endif
