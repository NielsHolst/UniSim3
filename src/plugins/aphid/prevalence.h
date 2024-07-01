/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef PREVALENCE_H
#define PREVALENCE_H
#include <base/box.h>

namespace aphid {

class Prevalence : public base::Box
{
public:
    Prevalence(QString name, Box *parent);
    void update();
private:
    // Inputs
    double aphidDensity, exposedDensity, cadaverDensity;
    // Outputs
    double exposed, cadavers;
};

}

#endif
