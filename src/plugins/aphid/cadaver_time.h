/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef CADAVERS_TIME_H
#define CADAVERS_TIME_H
#include <base/box.h>

namespace aphid {

class CadaverTime : public base::Box
{
public:
    CadaverTime(QString name, Box *parent);
    void update();
private:
    // Inputs
    bool isSporulating;
    double timeStep, rhAccelaration;
    // Outputs
    double step, total;
};

}

#endif
