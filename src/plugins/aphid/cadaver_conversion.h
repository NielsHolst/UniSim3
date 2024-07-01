/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef CADAVERS_CONVERSION_H
#define CADAVERS_CONVERSION_H
#include <base/box.h>

namespace aphid {

class CadaverConversion : public base::Box
{
public:
    CadaverConversion(QString name, Box *parent);
    void update();
private:
    // Inputs
    double
        succumbedApterousNymphs,
        succumbedAlateNymphs,
        succumbedApterousAdults,
        succumbedAlateAdults;
    // Outputs
    double cadavers, count;
};

}

#endif
