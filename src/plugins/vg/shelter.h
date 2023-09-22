/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SHELTER_H
#define SHELTER_H
#include "base/box.h"

namespace base {
    class BoxBuilder;
}

namespace vg {

class Shelter : public base::Box
{
public:
    Shelter(QString name, Box *parent);
    void amend();
private:
    // Inputs
    double transmissivityReduction;

};

} //namespace


#endif
