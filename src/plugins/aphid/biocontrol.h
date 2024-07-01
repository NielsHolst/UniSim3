/* Copyright 2018-2024 by
** Niels Holst, Aarhus University [niels.holst at agro.au.dk] and 
** Stéphanie Saussure, Norwegian Institute of Bioeconomy Research [stephanie.saussure at gmail.com].
** Released under the terms of the GNU Lesser General Public License version 3.0 or later.
** See: www.gnu.org/licenses/lgpl.html.
*/
#ifndef BIOCONTROL_H
#define BIOCONTROL_H
#include <base/box.h>

namespace aphid {

class Biocontrol : public base::Box
{
public:
    Biocontrol(QString name, Box *parent);
    void update();
private:
    // Inputs
    double
        aphidPressureWithoutF, aphidPressureWithF,
        yieldWithoutF, yieldWithF,
        cropGrowthStage,
        prevalence, cadaverPrevalence;
    // Outputs
    double
        aphidPressureDifference, yieldImprovement,
        percentageCadaversGs43, percentageCadaversGs61, percentageCadaversGs73,
        maxCadaverPrevalence, maxCadaverPrevalenceGS,
        maxPrevalence, maxPrevalenceGS;
};

}

#endif
