/* Copyright 2012-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Clement N. Mweya, National Institute for Medical Research, Tukuyu, Tanzania [cmweya at nimr.or.tz].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef SHEEP_FECUNDITY_H
#define SHEEP_FECUNDITY_H
#include <base/box.h>

namespace rvf {

class SheepFecundity : public base::Box
{
public:
    SheepFecundity(QString name, QObject *parent);
    void update();

private:
    // Inputs
    double lambDensity, adultDensity,
        birthRate, sexRatio, carryingCapacity;

    // Outputs
    double value;
};

} //namespace


#endif
