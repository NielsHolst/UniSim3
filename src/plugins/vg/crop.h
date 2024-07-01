/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef CROP_H
#define CROP_H

#include <base/box.h>

namespace vg {

class Crop : public base::Box
{
public:
    Crop(QString name, Box *parent);
    void amend();
    void reset();
private:
    // Inputs
    double lai, coverage,
        k, gammaStar, Jmax, lightRespiration,
        ballBerryIntercept, ballBerrySlope,
        Vcmax, alpha;
    bool trackPn;
};
} //namespace


#endif
