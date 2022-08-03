/* Copyright 2005-2021 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and
** Oliver Koerner, Leibniz-Institute of Vegetable and Ornamental Crops [koerner at igzev.de] and
** Jesper M. Aaslyng, Danish Technological Instutute [jeaa at teknologisk.dk].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html
*/
#ifndef PAR_BUDGET_H
#define PAR_BUDGET_H

#include <base/box.h>


namespace vg {

class ParBudget : public base::Box
{
public:
    ParBudget(QString name, QObject *parent);
    void reset();
    void update();
private:
    // Inputs
    double sunPar, growthLightsPar, indoorsTotalPar, Pn, cropCoverage;
    // Outputs
    double indoorsGrowthLightPar, indoorsSunPar, photosynthesis;
};

} //namespace

#endif
