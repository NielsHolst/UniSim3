/* Copyright 2005-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Oliver Körner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and 
** Jesper M. Aaslyng, HortiAdvice [jeaa at hortiadvice.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef CANOPY_PHOTOSYNTHESIS_H
#define CANOPY_PHOTOSYNTHESIS_H

#include <base/box.h>

namespace vg {

class CanopyPhotosynthesis : public base::Box
{
public:
    CanopyPhotosynthesis(QString name, Box *parent);
    void amend();
    void reset();
    void update();
private:
    // Inputs
    double lai, growthRespiration;
    QVector<double> leafAn, leafAg;
    bool trackPn;
    // Outputs
    double An, Ag, Ar, Pg, Pn, Pr;
    QVector<double> trackedPn;
};
} //namespace


#endif
