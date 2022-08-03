/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef AH_FROM_RH_H
#define AH_FROM_RH_H
#include <base/box.h>
#include <base/phys_math.h>


namespace vg {

class AhFromRh : public base::Box
{
public: 
    AhFromRh(QString name, Box *parent);
    void reset();
    void update();
private:
    // Inputs
    double temperature, rh;
    // Outputs
    double value;
};

} //namespace
#endif
