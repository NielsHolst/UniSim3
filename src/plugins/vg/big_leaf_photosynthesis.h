/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef BIG_LEAF_PHOTOSYNTHESIS_H
#define BIG_LEAF_PHOTOSYNTHESIS_H

#include <base/box.h>

namespace vg {

class BigLeafPhotosynthesis : public base::Box
{
public:
    BigLeafPhotosynthesis(QString name, Box *parent);
    void amend();
    void update();
private:
    // Inputs
    double lai, k, growthRespiration, leafAn, leafAg;
    // Outputs
    double An, Ag, Ar, Pg, Pn, Pr;
};
} //namespace


#endif
